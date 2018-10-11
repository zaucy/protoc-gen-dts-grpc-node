FROM zaucy/bazel:latest-gcc

WORKDIR /protoc-gen-dts-grpc-node

COPY WORKSPACE BUILD ./

COPY ./ ./

ENTRYPOINT bazel build -c opt :protoc-gen-dts-grpc-node && cp ./bazel-out/k8-opt/bin/protoc-gen-dts-grpc-node ./release/protoc-gen-dts-grpc-node-linux ; bazel shutdown
