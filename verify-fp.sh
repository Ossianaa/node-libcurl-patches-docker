#!/bin/bash
# verify-fp.sh — run the h3fp test image against fp.impersonate.pro and
# compare with the Chrome 151 fingerprint target.
set -e
cd "$(dirname "$0")"

IMG=curl_ngtcp2_h3fp_cli
URL=https://fp.impersonate.pro/api/http3

RAND="${1:-off}"
ARGS=(
  "fp-quic=scid=0"
  "fp-settings=1:65536;6:262144;7:100;51:1;GREASE"
  "fp-tp=12584:0x4f524947;9:103;1:30000;7:6291456;15:AUTO;4:15728640;GREASE;32:65536;3:1472;17:1@1,GREASE;8:100;6:6291456;12583:174718;5:6291456"
  "fp-tls=ciphers=1,2,3;alps=h3;grease=off;rand=$RAND"
  "curves=X25519MLKEM768:X25519:P-256:P-384"
  "perm=0,15,19,23,9,1,14,21,17,4,7"
  "sigalgs=0x0403,0x0804,0x0401,0x0503,0x0805,0x0501,0x0806,0x0601,0x0201"
)

docker run --rm "$IMG" //usr/local/bin/test-fp "$URL" "${ARGS[@]}" > measured.json 2> test-fp.err || { echo "TEST-FP FAILED"; cat test-fp.err; exit 1; }

node compare-fp.js measured.json chrome151-h3-fp.json; echo "compare exit: $?"
