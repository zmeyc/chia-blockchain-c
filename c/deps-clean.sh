#!/bin/bash
set -euo pipefail

script_root="$(cd "$(dirname "$(readlink "$([[ "${OSTYPE}" == linux* ]] && echo "-f")" "$0")")"; pwd)"

rm -rf "${script_root}/thirdparty" "${script_root}/libroot"

