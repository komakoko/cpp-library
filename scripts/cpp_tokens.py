"""Recognize C++ comments and literals, including raw string literals."""

import re

TOKEN = re.compile(
    r'(?:u8|u|U|L)?R"(?P<delimiter>[^\s()\\]{0,16})\(.*?\)(?P=delimiter)"'
    r'|/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
    re.S,
)


def mask_comments(code: str) -> tuple[str, set[int]]:
    """Mask comments with spaces and mark lines occupied by multiline literals."""
    parts = []
    literal_lines = set()
    start = 0
    line = 1
    for match in TOKEN.finditer(code):
        prefix = code[start:match.start()]
        token = match.group()
        parts.append(prefix)
        line += prefix.count("\n")
        newlines = token.count("\n")
        if token.startswith(("/*", "//")):
            parts.append(re.sub(r"[^\n]", " ", token))
        else:
            parts.append(token)
            if newlines:
                literal_lines.update(range(line, line + newlines + 1))
        line += newlines
        start = match.end()
    parts.append(code[start:])
    return "".join(parts), literal_lines
