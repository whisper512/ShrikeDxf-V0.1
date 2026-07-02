"""Cross-check disk vs vcxproj declared files for every project."""
import re
import pathlib

PROJECTS = [
    {
        "name": "ShrikeDxf",
        "vcxproj": r"D:\shrikedxf\ShrikeDxf\ShrikeDxf.vcxproj",
        "root": r"D:\shrikedxf\ShrikeDxf",  # project root for non-.. includes
        "vcxproj_dir": r"D:\shrikedxf\ShrikeDxf",  # directory of the vcxproj file
        "skip": {"x64"},
    },
    {
        "name": "libdxfrw",
        "vcxproj": r"D:\shrikedxf\libdxfrw\libdxfrw-master\vs2013\libdxfrw.vcxproj",
        "root": r"D:\shrikedxf\libdxfrw\libdxfrw-master",
        "vcxproj_dir": r"D:\shrikedxf\libdxfrw\libdxfrw-master\vs2013",
        "skip": {"Debug", "Release", "x64", ".vs"},
    },
    {
        "name": "dxflib",
        "vcxproj": r"D:\shrikedxf\dxflib\dxflib.vcxproj",
        "root": r"D:\shrikedxf\dxflib",
        "vcxproj_dir": r"D:\shrikedxf\dxflib",
        "skip": {"x64", "debug", "examples", "test"},
    },
]

INCLUDE_RE = re.compile(
    r'<(?:ClInclude|ClCompile|QtMoc|QtUic|QtRcc|None)\s+Include="([^"]+)"'
)

def resolve(inc, root, vcxproj_dir):
    p = pathlib.Path(inc)
    parts = p.parts
    # Compute leading ..
    leading_dots = 0
    for part in parts:
        if part == "..":
            leading_dots += 1
        else:
            break
    rest = pathlib.Path(*parts[leading_dots:])
    # The vcxproj file itself lives in <vcxproj_dir>; `..\foo` from vcxproj
    # is resolved relative to the vcxproj's directory, not the project root.
    base = pathlib.Path(vcxproj_dir)
    for _ in range(leading_dots):
        base = base.parent
    return (base / rest).resolve()

for proj in PROJECTS:
    print(f"\n========== {proj['name']} ==========")
    text = pathlib.Path(proj["vcxproj"]).read_text(encoding="utf-8", errors="ignore")
    declared = sorted(set(INCLUDE_RE.findall(text)))
    print(f"Declared count: {len(declared)}")
    root = pathlib.Path(proj["root"])
    vcxproj_dir = pathlib.Path(proj["vcxproj_dir"])
    skip = set(proj["skip"])
    missing = []
    for inc in declared:
        ap = resolve(inc, root, vcxproj_dir)
        if not ap.exists():
            missing.append((inc, str(ap)))
    if missing:
        print(f"-- Declared but missing on disk ({len(missing)}) --")
        for inc, ap in missing:
            print("  MISS", inc, "=>", ap)
    else:
        print("-- All declared files present on disk --")

    # Reverse: files on disk not declared
    disk = []
    for p in root.rglob("*"):
        if p.is_file() and not any(s in p.parts for s in skip):
            disk.append(p)
    declared_abs = {resolve(i, root, vcxproj_dir) for i in declared}
    not_declared = []
    for p in disk:
        if p.resolve() in declared_abs:
            continue
        rel = p.relative_to(root)
        not_declared.append(str(rel))
    if not_declared:
        print(f"-- On disk but NOT in vcxproj ({len(not_declared)}) --")
        for f in not_declared:
            print("  +", f)
    else:
        print("-- All disk files declared in vcxproj --")
