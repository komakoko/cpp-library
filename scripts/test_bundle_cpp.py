from pathlib import Path
import subprocess
import tempfile
import unittest

from bundle_cpp import bundle


class BundleTest(unittest.TestCase):
    def test_library_below_main_and_include_order(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "first.hpp").write_text(
                '#pragma once\n#include <bits/stdc++.h>\n'
                '// HEADER_COMMENT\n'
                'inline Before first() { return SETTING; }\n'
                '#define FROM_HEADER 2\n'
            )
            (root / "second.hpp").write_text(
                '#pragma once\n#include <bits/stdc++.h>\n'
                'inline After second() { return FROM_HEADER; }\n'
                'inline const char* text = R"tag(// literal\n\n'
                '#include <not_a_header>\n/* literal */)tag";\n'
            )
            source = root / "source.cpp"
            source.write_text(
                '// SOURCE_COMMENT\n#include <bits/stdc++.h>\n'
                '#define SETTING 3\nusing Before = int;\n'
                '#include "first.hpp"\nusing After = Before;\n'
                '#include "second.hpp"\n#include "first.hpp"\n\n'
                'int main() { std::cout << first() << second() << text; }\n'
            )
            result = bundle(source, [root]).decode()
            self.assertLess(result.index('int main()'), result.index('inline Before'))
            self.assertEqual(result.count('inline Before'), 1)
            self.assertEqual(result.count('#include <bits/stdc++.h>'), 1)
            self.assertIn('// SOURCE_COMMENT', result)
            self.assertNotIn('HEADER_COMMENT', result)
            self.assertNotIn('#line', result)
            self.assertNotIn(str(root), result)
            generated = root / "submission.cpp"
            generated.write_text(result)
            outputs = []
            for path in (source, generated):
                executable = path.with_suffix('')
                subprocess.run(['g++', '-std=c++23', str(path), '-o', str(executable)], check=True)
                outputs.append(subprocess.check_output([str(executable)]))
            self.assertEqual(outputs[0], outputs[1])

    def test_acl_remains_include_without_local_installation(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "main.cpp"
            code = '#include <atcoder/all>\n// my comment\nint main() {}\n'
            source.write_text(code)
            self.assertEqual(bundle(source, []), code.encode())

    def test_template_with_acl_only(self):
        library = Path(__file__).resolve().parent.parent
        template = library / 'template.cpp'
        original = template.read_bytes()
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            acl = library.parent / 'atcoder'
            if acl.is_dir():
                (root / 'atcoder').symlink_to(acl, target_is_directory=True)
            code = original.decode().replace('void solve() {', '''void solve() {
    ll value = 10;
    assert(chmin(value, 3LL) && chmax(value, 5LL));
    assert(ipow(2LL, 3) == 8 && mod(-1, 3) == 2 && isqrt(9) == 3);
    vector<int> a{3, 1, 2};
    sort(all(a));
    assert(sz(a) == 3 && a[0] == 1);
    ll total = 0;
    rep(i, sz(a)) total += a[i];
    rep(3) total += 1;
    rep(i, 1, 3) total += i;
    rep(i, 0, 4, 2) total += i;
    rrep(i, 0) assert(false);
    assert(total == 14);
    static_assert(mint::mod() == 1000000007);
''')
            code = code.replace('static_modint<998244353>', 'static_modint<1000000007>')
            source = root / 'main.cpp'
            source.write_text(code)
            result = bundle(source, [root], library).decode()
            self.assertIn('#include <atcoder/all>', result)
            self.assertNotIn('namespace atcoder {', result)
            self.assertLess(result.index('int main()'), result.index('bool chmin('))
            self.assertEqual(result.count('#if __INCLUDE_LEVEL__ == 0'), 1)
            self.assertEqual(sum(result.count(f'#include {macro}') for macro in ('__FILE__', '__FILE_NAME__')), 1)
            self.assertIn('KOMA_BUNDLE_SECTION', result)
            self.assertEqual(result.count('#include <bits/stdc++.h>'), 1)
            self.assertEqual(result.count('#define rep('), 1)
            self.assertLess(result.index('int main()'), result.index('#define rep('))
            generated = root / 'submission.cpp'
            generated.write_text(result)
            executable = root / 'check'
            subprocess.run(['g++', '-std=c++23', '-I', str(root), str(generated), '-o', str(executable)], check=True)
            subprocess.run([str(executable)], check=True)
        self.assertEqual(template.read_bytes(), original)

    def test_self_including_source_before_and_after_expansion(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'helper.hpp').write_text(
                '#pragma once\n#include <bits/stdc++.h>\n'
                '// HEADER_COMMENT\n'
                'inline Value helper() { return SETTING; }\n'
            )
            source = root / 'main.cpp'
            source.write_text('''#if __INCLUDE_LEVEL__ == 0
#include __FILE__

// SOURCE_COMMENT
int main() {
    std::vector<Value> values{helper(), 2};
    rep(i, values.size()) std::cout << values[i];
}
#else
#include <bits/stdc++.h>
using Value = int;
#define SETTING 3
#include "helper.hpp"
#include "helper.hpp"
#define rep(i, n) for (std::size_t i = 0; i < (n); ++i)
#endif
''')
            result = bundle(source, [root]).decode()
            self.assertEqual(sum(result.count(f'#include {macro}') for macro in ('__FILE__', '__FILE_NAME__')), 1)
            self.assertEqual(result.count('#include <bits/stdc++.h>'), 1)
            self.assertEqual(result.count('inline Value helper()'), 1)
            self.assertNotIn('KOMA_BUNDLE_SECTION', result)
            self.assertIn('// SOURCE_COMMENT', result)
            self.assertNotIn('HEADER_COMMENT', result)
            generated = root / 'submission.cpp'
            generated.write_text(result)
            for path in (source, generated):
                executable = path.with_suffix('')
                subprocess.run(['g++', '-std=c++23', str(path), '-o', str(executable)], check=True)
                self.assertEqual(subprocess.check_output([str(executable)]), b'32')


if __name__ == '__main__':
    unittest.main()
