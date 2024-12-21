#!/bin/bash
set -eouxf pipefail

#scons platform=linux target=template_debug debug_symbols=yes
scons platform=linux
