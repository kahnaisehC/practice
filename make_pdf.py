#!/usr/bin/env python3
"""Build a printable PDF from syntax-highlighted code snippets.

Examples
--------
    # Default snippet set -> snippets.pdf
    python3 make_pdf.py

    # Choose your own files / directories
    python3 make_pdf.py teoria_numeros_general.cpp notes/ -o cheat_sheet.pdf

    # Every source file in the repository
    python3 make_pdf.py --all

    # Pure black-and-white (cheaper to print, no colors)
    python3 make_pdf.py --no-color

Requires: Pygments (pip install pygments) and an HTML -> PDF engine.
LibreOffice (`soffice`) is used by default; WeasyPrint / wkhtmltopdf /
Chromium are used automatically if they are installed.
"""

from __future__ import annotations

import argparse
import datetime as dt
import html
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

try:
    from pygments import highlight
    from pygments.formatters import HtmlFormatter
    from pygments.lexers import TextLexer, guess_lexer_for_filename
    from pygments.util import ClassNotFound
except ImportError:  # pragma: no cover
    sys.exit("Pygments is required: python3 -m pip install --user pygments")

CODE_EXTENSIONS = {
    ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx",
    ".py", ".java", ".js", ".jsx", ".ts", ".tsx", ".rs", ".go",
    ".kt", ".kts", ".cs", ".php", ".rb", ".swift", ".scala",
    ".sql", ".sh", ".bash", ".lua", ".m", ".r",
}

# Files that make a useful default cheat sheet.
DEFAULT_FILES = [
    "teoria_numeros_general.cpp",
    "combinatoria_general.cpp",
    "Geometria_mejorado.cpp",
    "complete_notes.cpp",
    "notes/snippet.py",
    "notes/sort_lambda.cpp",
]

SKIP_DIRS = {".git", ".hg", ".svn", "__pycache__", "build", "dist", "node_modules"}


def read_text(path: Path) -> str:
    """Read a source file, tolerating non-UTF-8 encodings."""
    raw = path.read_bytes()
    for encoding in ("utf-8", "latin-1"):
        try:
            return raw.decode(encoding)
        except UnicodeDecodeError:
            continue
    return raw.decode("utf-8", errors="replace")


def collect_files(args: list[str], include_all: bool) -> list[Path]:
    """Resolve CLI arguments (files and directories) into a file list."""
    root = Path.cwd()
    files: list[Path] = []

    if args:
        for arg in args:
            path = Path(arg).expanduser()
            if path.is_dir():
                for candidate in sorted(path.rglob("*")):
                    if candidate.is_file() and candidate.suffix.lower() in CODE_EXTENSIONS:
                        if not any(part in SKIP_DIRS for part in candidate.parts):
                            files.append(candidate.resolve())
            elif path.is_file():
                files.append(path.resolve())
            else:
                sys.exit(f"Not found: {arg}")
    elif include_all:
        this_file = Path(__file__).resolve()
        for candidate in sorted(root.rglob("*")):
            if candidate.is_file() and candidate.suffix.lower() in CODE_EXTENSIONS:
                if candidate.resolve() == this_file:
                    continue
                if not any(part in SKIP_DIRS for part in candidate.relative_to(root).parts):
                    files.append(candidate.resolve())
    else:
        for name in DEFAULT_FILES:
            path = root / name
            if path.is_file():
                files.append(path.resolve())

    # Remove duplicates while preserving order.
    seen: set[Path] = set()
    unique: list[Path] = []
    for path in files:
        if path not in seen:
            seen.add(path)
            unique.append(path)
    return unique


def display_name(path: Path, root: Path) -> str:
    try:
        return str(path.relative_to(root))
    except ValueError:
        return str(path)


def build_html(files: list[Path], title: str, font_size: float,
               line_height: float, color: bool, landscape: bool) -> str:
    root = Path.cwd()
    orientation = " landscape" if landscape else ""
    style = f"""
@page {{
  size: A4{orientation};
  margin: 12mm 11mm;
  @bottom-center {{
    content: counter(page) " / " counter(pages);
    font-family: "DejaVu Sans", sans-serif;
    font-size: 7pt;
    color: #777;
  }}
}}
body {{
  font-family: "DejaVu Sans Mono", "Liberation Mono", "Courier New", monospace;
  font-size: {font_size}pt;
  line-height: {line_height};
  color: #111;
  margin: 0;
}}
.doc-title {{
  font-family: "DejaVu Sans", sans-serif;
  font-size: 13pt;
  font-weight: bold;
  border-bottom: 2px solid #333;
  padding-bottom: 1.5mm;
  margin: 0;
}}
.doc-sub {{
  font-family: "DejaVu Sans", sans-serif;
  font-size: 7.5pt;
  color: #666;
  margin: 1mm 0 5mm 0;
}}
h1, h2 {{
  font-family: "DejaVu Sans", sans-serif;
  font-size: 10.5pt;
  margin: 0 0 1.5mm 0;
  padding-bottom: 1mm;
  border-bottom: 1px solid #999;
}}
/* LibreOffice only honours page breaks on h1, so the first file uses h2. */
h1 {{ page-break-before: always; }}
h1 .meta, h2 .meta {{
  font-size: 7.5pt;
  font-weight: normal;
  color: #666;
}}
pre {{
  white-space: pre-wrap;
  overflow-wrap: break-word;
  word-wrap: break-word;
  margin: 0;
  background: transparent;
}}
"""
    parts = [
        "<!DOCTYPE html>",
        '<html lang="en"><head><meta charset="utf-8">',
        f"<title>{html.escape(title)}</title>",
        f"<style>{style}",
    ]
    if color:
        # No ".highlight" prefix: LibreOffice does not resolve descendant
        # selectors when importing HTML, but plain ".k { ... }" works.
        parts.append(HtmlFormatter(style="default").get_style_defs())
        parts.append(".highlight { background: transparent !important; }")
    parts.append("</style></head><body>")

    if title:
        today = dt.date.today().isoformat()
        parts.append(f'<div class="doc-title">{html.escape(title)}</div>')
        parts.append(
            f'<div class="doc-sub">{len(files)} file(s) &middot; generated {today}</div>'
        )

    for index, path in enumerate(files):
        code = read_text(path)
        name = html.escape(display_name(path, root))
        lines = code.count("\n") + (0 if code.endswith("\n") or not code else 1)
        tag = "h2" if index == 0 else "h1"
        parts.append(
            f'<{tag}>{name} '
            f'<span class="meta">&mdash; {lines} lines</span></{tag}>'
        )

        try:
            lexer = guess_lexer_for_filename(str(path), code)
        except ClassNotFound:
            lexer = TextLexer()

        highlighted = (
            highlight(code, lexer, HtmlFormatter(nowrap=True))
            if color
            else html.escape(code)
        )
        parts.append(f'<pre class="highlight">{highlighted}</pre>')

    parts.append("</body></html>")
    return "\n".join(parts)


def html_to_pdf(html_file: Path, pdf_file: Path, engine: str | None) -> None:
    """Convert an HTML file to PDF using whichever engine is available."""
    candidates: list[str]
    if engine:
        candidates = [engine]
    else:
        candidates = []
        if shutil.which("weasyprint"):
            candidates.append("weasyprint")
        if shutil.which("soffice"):
            candidates.append("soffice")
        if shutil.which("wkhtmltopdf"):
            candidates.append("wkhtmltopdf")
        for name in ("chromium", "chromium-browser", "google-chrome"):
            if shutil.which(name):
                candidates.append(name)
                break

    if not candidates:
        sys.exit(
            "No HTML -> PDF engine found. Install LibreOffice (soffice) or "
            "WeasyPrint (python3 -m pip install --user weasyprint)."
        )

    last_error: Exception | None = None
    for name in candidates:
        try:
            if name == "weasyprint":
                subprocess.run(
                    [shutil.which("weasyprint"), str(html_file), str(pdf_file)],
                    check=True, capture_output=True,
                )
                return
            if name == "soffice":
                with tempfile.TemporaryDirectory(prefix="lo-profile-") as profile:
                    profile_uri = Path(profile).as_uri()
                    subprocess.run(
                        [
                            shutil.which("soffice"), "--headless",
                            f"-env:UserInstallation={profile_uri}",
                            "--convert-to", "pdf",
                            "--outdir", str(pdf_file.parent),
                            str(html_file),
                        ],
                        check=True, capture_output=True, timeout=180,
                    )
                produced = pdf_file.parent / (html_file.stem + ".pdf")
                if produced != pdf_file:
                    produced.replace(pdf_file)
                return
            if name == "wkhtmltopdf":
                subprocess.run(
                    [shutil.which("wkhtmltopdf"), "--enable-local-file-access",
                     str(html_file), str(pdf_file)],
                    check=True, capture_output=True,
                )
                return
            subprocess.run(
                [shutil.which(name), "--headless", "--no-sandbox",
                 f"--print-to-pdf={pdf_file}", html_file.as_uri()],
                check=True, capture_output=True, timeout=180,
            )
            return
        except (subprocess.CalledProcessError, subprocess.TimeoutExpired,
                OSError) as exc:
            last_error = exc

    sys.exit(f"Could not create PDF: {last_error}")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Create a printable PDF of syntax-highlighted code snippets.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument(
        "paths", nargs="*",
        help="files or directories to include (default: the notes/snippet set)",
    )
    parser.add_argument("-o", "--output", default="snippets.pdf",
                        help="output PDF path (default: snippets.pdf)")
    parser.add_argument("--all", action="store_true",
                        help="include every source file in the current directory")
    parser.add_argument("--title", default="Competitive Programming - Code Snippets",
                        help='document title; use --title "" to omit it')
    parser.add_argument("--font-size", type=float, default=8.0,
                        help="code font size in pt (default: 8)")
    parser.add_argument("--line-height", type=float, default=1.25,
                        help="line height multiplier (default: 1.25)")
    parser.add_argument("--no-color", action="store_true",
                        help="black-and-white output (no syntax highlighting)")
    parser.add_argument("--landscape", action="store_true",
                        help="use A4 landscape instead of portrait")
    parser.add_argument("--engine", choices=["weasyprint", "soffice",
                                             "wkhtmltopdf", "chromium"],
                        help="force a specific HTML -> PDF engine")
    parser.add_argument("--keep-html", action="store_true",
                        help="keep the intermediate HTML next to the PDF")
    args = parser.parse_args()

    files = collect_files(args.paths, args.all)
    if not files:
        sys.exit("No input files found.")

    output = Path(args.output).expanduser().resolve()
    if output.suffix.lower() != ".pdf":
        output = output.with_suffix(".pdf")
    output.parent.mkdir(parents=True, exist_ok=True)

    document = build_html(
        files=files,
        title=args.title,
        font_size=args.font_size,
        line_height=args.line_height,
        color=not args.no_color,
        landscape=args.landscape,
    )

    with tempfile.TemporaryDirectory(prefix="snippets-") as tmp:
        # Use the output name so LibreOffice writes the PDF we asked for and
        # cannot overwrite an unrelated snippets.pdf in the output directory.
        tmp_html = Path(tmp) / (output.stem + ".html")
        tmp_html.write_text(document, encoding="utf-8")
        html_to_pdf(tmp_html, output, args.engine)
        if args.keep_html:
            output.with_suffix(".html").write_text(document, encoding="utf-8")

    print(f"Created {output} ({len(files)} files)")


if __name__ == "__main__":
    main()
