cc_binary(
  name = "protoc-gen-dts-grpc-node",
  linkstatic = True,
  linkopts = ["-static"],
  srcs = [
    "src/generator.cc",
    "src/dts-grpc-node-generator-options.cc",
    "src/dts-grpc-node-generator-options.hh",
    "src/dts-grpc-node-generator.cc",
    "src/dts-grpc-node-generator.hh",
  ],
  deps = [
    "@com_google_protobuf//:protoc_lib",
  ],
)
