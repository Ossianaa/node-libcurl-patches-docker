## libcurl module patches

> Some of the patch reference [curl-impersonate](https://github.com/lwthiker/curl-impersonate) [curl-impersonate](https://github.com/lexiforest/curl-impersonate)

### HTTP/3 fingerprint patches (applied in this order, after the base patches)

| patch | applies to | purpose |
| :---: | :---: | :--- |
| `boringssl-fp.patch` | boringssl (after `boringssl-….patch`) | per-ctx TLS-level GREASE on/off, configurable ALPS protocol/settings, 0-based decimal extension permutation |
| `nghttp3-fp.patch` | nghttp3 | fingerprint SETTINGS list (`fp_iv`/`fp_ivlen`) sent verbatim in wire order |
| `ngtcp2-fp.patch` | ngtcp2 | custom transport-parameter list in wire order (`ngtcp2_conn_set_local_transport_params_custom`) |
| `curl-h3fp.patch` | curl (after base + `curl-socks5-udp.patch`) | `CURLOPT_HTTP3_FP_QUIC/SETTINGS/TRANSPORT_PARAMS/TLS` (331-334) + QUIC brotli cert compression |
| `curl-fix-session-ticket.patch` | curl (last) | declare `Curl_ssl_session_get_ticket` in `vtls.h` (missing prototype breaks clang builds, which error on implicit declarations; gcc C89 builds only warned) |

See `lib/vquic/curl_ngtcp2.c` (quic_fp_parse_*) for the option string formats.

## module version
|      module       | version |
| :-----------------: | :-----: |
| curl | [d755a5f7c009dd63a61b2c745180d8ba937cbfeb](https://github.com/curl/curl/tree/d755a5f7c009dd63a61b2c745180d8ba937cbfeb) |
|  boringssl  | [1c7d52ef3e3f373302cb957089fa783d1e5fd8cd](https://github.com/google/boringssl/blob/1c7d52ef3e3f373302cb957089fa783d1e5fd8cd)  |
|  nghttp2  | [05b792901933664e7aeb7ebb8c87db81123d484a](https://github.com/nghttp2/nghttp2/tree/05b792901933664e7aeb7ebb8c87db81123d484a)  |
|  ngtcp2  | [v1.0.1](https://github.com/ngtcp2/ngtcp2/tree/v1.0.1)  |
|  nghttp3  | [v1.0.0](https://github.com/ngtcp2/nghttp3/tree/v1.0.0)  |
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
