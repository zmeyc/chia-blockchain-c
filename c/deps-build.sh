#!/bin/bash
set -euo pipefail

script_root="$(cd "$(dirname "$(readlink "$([[ "${OSTYPE}" == linux* ]] && echo "-f")" "$0")")"; pwd)"

build_bls_signatures() {
  [ ! -f "${script_root}/thirdparty/.bls-signatures-built" ] || return 0

  echo "Building bls-signatures"
  pushd "${script_root}/thirdparty/bls-signatures"

  mkdir -p build
  pushd build
  cmake \
    -DCMAKE_INSTALL_PREFIX="${script_root}/libroot" \
    -DBUILD_BLS_PYTHON_BINDINGS=0 \
    ..
  cmake --build . --parallel "$(nproc)"
  cmake --install .

  # build
  popd
  # thirdparty/bls-signatures
  popd

  touch "${script_root}/thirdparty/.bls-signatures-built"
}

build_chiavdf() {
  [ ! -f "${script_root}/thirdparty/.chiavdf-built" ] || return 0

  echo "Building chiavdf"
  pushd "${script_root}/thirdparty/chiavdf"

  mkdir -p build
  pushd build
  cmake \
    -DCMAKE_INSTALL_PREFIX="${script_root}/libroot" \
    ../src
  cmake --build . --parallel "$(nproc)"
  cmake --install .

  # build
  popd
  # thirdparty/chiavdf
  popd

  touch "${script_root}/thirdparty/.chiavdf-built"
}

[ -d "${script_root}/thirdparty" ] || ( echo "Please download dependencies by running ./deps-download.sh"; exit 1 )

mkdir -p libroot

build_bls_signatures
build_chiavdf

