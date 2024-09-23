#!/bin/bash

set -e

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <new-version>"
    exit 1
fi

REPO_ROOT=$(dirname "$0")/..

sed -i "s/^version: \".*\"/version: \"$1\"/" "$REPO_ROOT/idf_component.yml"
sed -i "s/\"version\": \".*\"/\"version\": \"$1\"/" "$REPO_ROOT/library.json"
