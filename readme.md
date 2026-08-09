## libcurl module patches

> Some of the patch reference [curl-impersonate](https://github.com/lwthiker/curl-impersonate) [curl-impersonate](https://github.com/lexiforest/curl-impersonate)

### Patches (one per module, applied once to the pristine checkout)

| patch | applies to | purpose |
| :---: | :---: | :--- |
| `curl-d755a5f7c009dd63a61b2c745180d8ba937cbfeb.patch` | curl | curl-impersonate base profile; SOCKS5 UDP relay for QUIC; HTTP/3 fingerprint options (`CURLOPT_HTTP3_FP_QUIC/SETTINGS/TRANSPORT_PARAMS/TLS` 331-334) + QUIC brotli cert compression; HTTP/2 PRIORITY frame fix (`h2_pri_spec()` init + per-stream once-only submission — the spec was uninitialized stack → UB, the frame silently vanished on some platforms); `Curl_ssl_session_get_ticket` prototype in `vtls.h` |
| `boringssl-1c7d52ef3e3f373302cb957089fa783d1e5fd8cd.patch` | boringssl | base patch; per-ctx TLS-level GREASE on/off, configurable ALPS protocol/settings, 0-based decimal extension permutation |
| `nghttp3-868e81b343d1350597b47f78a5958d60c3695aa4.patch` | nghttp3 | fingerprint SETTINGS list (`fp_iv`/`fp_ivlen`) sent verbatim in wire order; MSVC build compatibility |
| `ngtcp2-843aa72100b508e192f119439a943028b0c6f030.patch` | ngtcp2 | custom transport-parameter list in wire order (`ngtcp2_conn_set_local_transport_params_custom`); MSVC build compatibility |

See `lib/vquic/curl_ngtcp2.c` (quic_fp_parse_*) for the option string formats.

### 勘误: fp-tp value encodings

| fp-tp value | encoding on the wire |
| :--- | :--- |
| `18258:1` (google_version) | **NOT a varint** — Chrome sends its initial QUIC version label as a 4-byte big-endian value, so `18258:1` → `00 00 00 01` (fp.impersonate.pro previously reported `"value": "0x01"`; now `"value": 1`). Decimal or 0x both accepted: `18258:1` / `18258:0x00000001`; values > 32-bit are rejected |
| `id:chosen@v1,v2,…` (e.g. `17:1@1,GREASE`, `16741339:1@1,GREASE`) | version_information structure for any id: chosen version 4-byte BE + one 4-byte value per available version; `GREASE` versions use the `0x?a?a?a?a` pattern |
| `id:0x…` (e.g. `12584:0x4f524947` google_connection_options) | the hex IS the raw value bytes, not a varint |
| `15:AUTO` (original_destination_connection_id) | sends the generated DCID bytes |
| `GREASE` | Chrome's fixed grease id `0x10bd660917c6e651` with a random 2-byte value |

## module version
|      module       | version |
| :-----------------: | :-----: |
| curl | [d755a5f7c009dd63a61b2c745180d8ba937cbfeb](https://github.com/curl/curl/tree/d755a5f7c009dd63a61b2c745180d8ba937cbfeb) |
|  boringssl  | [1c7d52ef3e3f373302cb957089fa783d1e5fd8cd](https://github.com/google/boringssl/blob/1c7d52ef3e3f373302cb957089fa783d1e5fd8cd)  |
|  nghttp2  | [05b792901933664e7aeb7ebb8c87db81123d484a](https://github.com/nghttp2/nghttp2/tree/05b792901933664e7aeb7ebb8c87db81123d484a)  |
|  ngtcp2  | [843aa72100b508e192f119439a943028b0c6f030](https://github.com/ngtcp2/ngtcp2/tree/843aa72100b508e192f119439a943028b0c6f030) (v1.25.0)  |
|  nghttp3  | [868e81b343d1350597b47f78a5958d60c3695aa4](https://github.com/ngtcp2/nghttp3/tree/868e81b343d1350597b47f78a5958d60c3695aa4) (v1.18.0)  |
| zlib | [04f42ceca40f73e2978b50e93806c2a18c1281fc](https://github.com/madler/zlib/tree/04f42ceca40f73e2978b50e93806c2a18c1281fc) |
| zstd  | [a58b48ef0e543980888a4d9d16c9072ff22135ca](https://github.com/facebook/zstd/tree/a58b48ef0e543980888a4d9d16c9072ff22135ca) |
|  brotli  | [71fe6cac061ac62c0241f410fbd43a04a6b4f303](https://github.com/google/brotli/tree/71fe6cac061ac62c0241f410fbd43a04a6b4f303)  |
------------

## docker build

```bash
docker build . -f Dockerfile-arm64-apple-darwin -t curl_arm64-apple-darwin

docker run --rm -v /path/to/your:/output curl_arm64-apple-darwin
```

Now `libcrypto.a` `libssl.a` `libzstd.a` `libngtcp2.a` `libngtcp2_crypto_boringssl.a` `libnghttp3.a` `libcurl.a` are generated in `/path/to/your`.

### Windows
```bash
docker build . -f Dockerfile-x86_64-pc-windows-msvc -t curl_x86_64-pc-windows-msvc

docker run --rm -v /path/to/your:/home/wine/.wine/drive_c/output curl_x86_64-pc-windows-msvc
```

Now `libcurl.dll` `libcurl_imp.lib` are generated in `/path/to/your`.
