#!/bin/bash

set -e

ARCHIVE_URL='https://cdn.intra.42.fr/document/document/28303/libftpp.1.0.tgz'

curl -L $ARCHIVE_URL | tar -xz

patch -d libftpp -p1 < patch.diff

find libftpp -type f -empty -delete

mv libftpp main || (rm -dfr libftpp; exit 1)
mv main src/ || (rm -dfr main; exit 1)
