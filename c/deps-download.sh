#!/bin/bash
set -euo pipefail

script_root="$(cd "$(dirname "$(readlink "$([[ "${OSTYPE}" == linux* ]] && echo "-f")" "$0")")"; pwd)"

mkdir -p "${script_root}/thirdparty"
cd "${script_root}/thirdparty"

dep_clone() {
  local dir="$1"
  local repo="$2"
  local branch="$3"

  if [ ! -d "${script_root}/thirdparty/${dir}" ]; then
    printf "Cloning %s...\n" "${dir}"
    git clone -b "${branch}" --recursive "${repo}" "${script_root}/thirdparty/${dir}"

    if [ -f "${script_root}/thirdparty-patches/${dir}.patch" ]; then
      printf "Patching %s...\n" "${dir}"
      patch -d"${script_root}/thirdparty/${dir}" -p1 < "${script_root}/thirdparty-patches/${dir}.patch"
    fi
  else
    printf "Dependency '%s' already exists\n" "${dir}"
  fi
}

dep_clone "bls-signatures" "https://github.com/Chia-Network/bls-signatures.git" "1.0.5"
dep_clone "chiavdf" "https://github.com/Chia-Network/chiavdf.git" "1.0.2"

