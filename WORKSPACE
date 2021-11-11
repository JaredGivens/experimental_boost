
workspace(name = "experimental_boost")

new_local_repository(
    name = "boost",
    path = "/usr/local/Cellar/boost/1.76.0",
    build_file = "boost.BUILD",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "eigen",
    build_file = "//:eigen.BUILD",
    sha256 = "3a66f9bfce85aff39bc255d5a341f87336ec6f5911e8d816dd4a3fdc500f8acf",
    url = "https://bitbucket.org/eigen/eigen/get/c5e90d9.tar.gz",
    strip_prefix="eigen-eigen-c5e90d9e764e"
)