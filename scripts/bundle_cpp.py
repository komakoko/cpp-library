"""Place expanded local headers below the source using self-inclusion."""

from pathlib import Path
import re

from cpp_tokens import mask_comments

_INCLUDE = re.compile(r'^\s*#\s*include\s*([<"])([^>"\n]+)[>"]')


def _is_self_including(code: str) -> bool:
    lines = [line.strip() for line in code.splitlines() if line.strip()]
    if not lines or not re.fullmatch(r'#\s*if\s+__INCLUDE_LEVEL__\s*==\s*0', lines[0]):
        return False
    depth = 1
    for line in lines[1:]:
        if depth == 1:
            if re.fullmatch(r'#\s*include\s+__(?:FILE|FILE_NAME)__', line):
                return True
            if re.match(r'#\s*(else|elif|endif)\b', line):
                return False
        if re.match(r'#\s*(if|ifdef|ifndef)\b', line):
            depth += 1
        elif re.match(r'#\s*endif\b', line):
            depth -= 1
    return False


def bundle(source: Path, include_paths: list[Path], library: Path | None = None) -> bytes:
    expanded: set[Path] = set()
    active: set[Path] = set()
    source = source.resolve()
    bits_included = False
    sections: list[str] = []

    def resolve_header(path: Path, number: int, name: str, quoted: bool) -> Path | None:
        if name.startswith("atcoder/"):
            return None
        candidates = [path.parent / name] if quoted else []
        candidates.extend(root / name for root in include_paths)
        if library is not None and name.startswith("koma/"):
            candidates.append(library / name.removeprefix("koma/"))
        for candidate in candidates:
            if candidate.is_file():
                return candidate
        if quoted or name.startswith("koma/"):
            raise FileNotFoundError(f"{path}:{number}: header not found: {name}")
        return None

    def expand(path: Path, conditional_depth: int = 0) -> str:
        nonlocal bits_included
        path = path.resolve()
        if path in active:
            raise ValueError(f"cyclic include: {path}")
        if path in expanded:
            return ""
        active.add(path)
        expanded.add(path)
        code = path.read_text(encoding="utf-8")
        uncommented, literal_lines = mask_comments(code)
        self_including = path == source and _is_self_including(uncommented)
        output = []
        continued = False
        for number, (line, clean) in enumerate(zip(code.splitlines(), uncommented.splitlines()), 1):
            if path != source:
                line = clean
                keep_line = bool(line.strip()) or continued or number in literal_lines
                continued = line.endswith("\\")
                if not keep_line:
                    continue
            if number in literal_lines:
                output.append(line + "\n")
                continue
            if re.match(r'^\s*#\s*(if|ifdef|ifndef)\b', clean):
                conditional_depth += 1
            elif re.match(r'^\s*#\s*endif\b', clean):
                conditional_depth -= 1
            if re.match(r'^\s*#\s*pragma\s+once\b', clean):
                continue
            match = _INCLUDE.match(clean)
            if not match:
                output.append(line + "\n")
                continue

            delimiter, name = match.groups()
            include_depth = conditional_depth - int(self_including)
            if name == "bits/stdc++.h" and include_depth == 0:
                if bits_included:
                    continue
                bits_included = True

            header = resolve_header(path, number, name, delimiter == '"')
            if header is None:
                output.append(line + "\n")
                continue
            expanded_header = expand(header, include_depth)
            if path == source and not self_including and expanded_header:
                sections.append(expanded_header)
                output.append(
                    f'#define KOMA_BUNDLE_SECTION {len(sections)}\n'
                    '#include __FILE_NAME__\n'
                    '#undef KOMA_BUNDLE_SECTION\n'
                )
            else:
                output.append(expanded_header)
        active.remove(path)
        return "".join(output)

    program = expand(source)
    if not sections:
        return program.encode("utf-8")
    section_lines = []
    for number, section in enumerate(sections, 1):
        directive = "if" if number == 1 else "elif"
        section_lines.append(f'#{directive} KOMA_BUNDLE_SECTION == {number}\n')
        section_lines.append(section)
    section_lines.append('#endif\n')
    return (
        '#if __INCLUDE_LEVEL__ == 0\n' + program
        + '#else\n' + ''.join(section_lines) + '#endif\n'
    ).encode("utf-8")
