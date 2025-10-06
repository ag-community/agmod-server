import argparse
import os
import re
import shutil
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(description='Prepares CMake install results for zipping')
    parser.add_argument('--build-dir', required=True, help='CMake build directory')
    parser.add_argument('--install-dir', required=True, help='CMake install directory')
    parser.add_argument('--artifact-dir', required=True, help='Output artifact directory. A new directory will be created there')
    parser.add_argument('--target', required=True, choices=('client', 'server'), help='Target type for packaging')
    parser.add_argument('--suffix', required=True, help='Name suffix')
    parser.add_argument('--allow-mod', action=argparse.BooleanOptionalAction, help='Allow version to have .m suffix')
    
    args = parser.parse_args()
    build_dir = Path(args.build_dir)
    install_dir = Path(args.install_dir)
    artifact_dir = Path(args.artifact_dir)

    # Assemble the artifact name
    artifact_name = f'AG-{args.target}-{args.suffix}'
    print(f'artifact_name = {artifact_name}')

    # Copy install files to artifact dir
    artifact_inner_dir = artifact_dir / artifact_name / 'ag_addon'
    artifact_inner_dir.parent.mkdir(parents=True, exist_ok=True)
    shutil.copytree(install_dir, artifact_inner_dir)

    # Pass artifact name to GitHub Actions
    if 'GITHUB_OUTPUT' in os.environ:
        print(f'Passing artifact name to GitHub Actions as artifact_name')
        with open(os.environ['GITHUB_OUTPUT'], 'a', encoding='utf-8') as f:
            f.write(f'artifact_name={artifact_name}\n')
    else:
        print(f'Not running on GitHub Actions')


main()
