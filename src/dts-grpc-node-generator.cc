#include "dts-grpc-node-generator.hh"

#include <cctype>
#include <set>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::FileDescriptor;
using google::protobuf::MethodDescriptor;
using google::protobuf::ServiceDescriptor;
using google::protobuf::compiler::CodeGenerator;
using google::protobuf::compiler::GeneratorContext;
using google::protobuf::compiler::ParseGeneratorParameter;
using google::protobuf::compiler::PluginMain;
using google::protobuf::io::Printer;
using google::protobuf::io::ZeroCopyOutputStream;

namespace {
  std::string firstCharToLower(std::string str) {
    if(str.length() > 0) {
      str[0] = std::tolower(str[0]);
    }

    return str;
  }

  std::string firstCharToUpper(std::string str) {
    if(str.length() > 0) {
      str[0] = std::toupper(str[0]);
    }

    return str;
  }

  std::string removePathExtname(const std::string& path) {
    auto dotIndex = path.find_last_of('.');

    if(dotIndex != std::string::npos) {
      return path.substr(0, dotIndex);
    }

    return path;
  }
}

bool DtsGrpcNodeGenerator::PrintServiceImplementationInterface
  ( google::protobuf::io::Printer&              printer
  , const DtsGrpcNodeGeneratorOptions&          options
  , const google::protobuf::ServiceDescriptor*  service
  , std::string*                                error
  ) const
{

  std::map<std::string, std::string> vars = {
    {"ServiceName", service->name()}
  };

  printer.Print(vars, "export interface I$ServiceName$Implementation {\n");
  printer.Indent();

  auto methodCount = service->method_count();

  for(auto i=0; methodCount > i; ++i) {
    auto method = service->method(i);
    auto inputType = method->input_type();
    auto outputType = method->output_type();

    vars.insert({"MethodName", firstCharToLower(method->name())});
    vars.insert({"RequestName", inputType->full_name()});
    vars.insert({"ResponseName", outputType->full_name()});
    
    if(method->client_streaming() && method->server_streaming()) {
      printer.Print(vars, "$MethodName$: grpc.handleBidiStreamingCall<$RequestName$, $ResponseName$>;\n");
    } else
    if(method->client_streaming()) {
      printer.Print(vars, "$MethodName$: grpc.handleClientStreamingCall<$RequestName$, $ResponseName$>;\n");
    } else
    if(method->server_streaming()) {
      printer.Print(vars, "$MethodName$: grpc.handleServerStreamingCall<$RequestName$, $ResponseName$>;\n");
    } else {
      printer.Print(vars, "$MethodName$: grpc.handleUnaryCall<$RequestName$, $ResponseName$>;\n");
    }
  }

  printer.Outdent();
  printer.Print(vars, "}\n\n");

  return true;
}

bool DtsGrpcNodeGenerator::PrintServiceDefinitionInterface
  ( google::protobuf::io::Printer&              printer
  , const DtsGrpcNodeGeneratorOptions&          options
  , const google::protobuf::ServiceDescriptor*  service
  , std::string*                                error
  ) const
{
  std::map<std::string, std::string> vars = {
    {"ServiceName", service->name()}
  };

  printer.Print(vars, "export interface I$ServiceName$ServiceDefinition extends grpc.ServiceDefinition<I$ServiceName$Implementation> {\n");
  printer.Indent();

  auto methodCount = service->method_count();

  for(auto i=0; methodCount > i; ++i) {
    auto method = service->method(i);
    auto inputType = method->input_type();
    auto outputType = method->output_type();

    vars.insert({"MethodName", firstCharToLower(method->name())});
    vars.insert({"RequestName", inputType->full_name()});
    vars.insert({"ResponseName", outputType->full_name()});
    
    printer.Print(vars, "$MethodName$: grpc.MethodDefinition<$RequestName$, $ResponseName$>;\n");
  }

  printer.Outdent();
  printer.Print(vars, "}\n\n");

  printer.Print(vars, "export const $ServiceName$Service: I$ServiceName$ServiceDefinition;\n\n");

  return true;
}

bool DtsGrpcNodeGenerator::PrintServiceClientClass
  ( google::protobuf::io::Printer&              printer
  , const DtsGrpcNodeGeneratorOptions&          options
  , const google::protobuf::ServiceDescriptor*  service
  , std::string*                                error
  ) const
{
  std::map<std::string, std::string> vars = {
    {"ServiceName", service->name()}
  };

  printer.Print(vars, "export class $ServiceName$Client extends grpc.Client {\n");
  printer.Indent();

  auto methodCount = service->method_count();

  for(auto i=0; methodCount > i; ++i) {
    auto method = service->method(i);
    auto inputType = method->input_type();
    auto outputType = method->output_type();

    vars.insert({"MethodName", firstCharToLower(method->name())});
    vars.insert({"RequestName", inputType->full_name()});
    vars.insert({"ResponseName", outputType->full_name()});
    
    if(method->client_streaming() && method->server_streaming()) {

    } else
    if(method->client_streaming()) {

    } else
    if(method->server_streaming()) {

    } else {
      printer.Print(vars, "$MethodName$\n");
      printer.Indent();
      printer.Print(vars, "( request: $RequestName$\n");
      printer.Print(vars, ", callback: grpc.requestCallback<$ResponseName$>\n");
      printer.Print(vars, "): void;\n");
      printer.Outdent();

      printer.Print(vars, "$MethodName$\n");
      printer.Indent();
      printer.Print(vars, "( request: $RequestName$\n");
      printer.Print(vars, ", metadata: grpc.Metadata | null\n");
      printer.Print(vars, ", callback: grpc.requestCallback<$ResponseName$>\n");
      printer.Print(vars, "): void;\n");
      printer.Outdent();

      printer.Print(vars, "$MethodName$\n");
      printer.Indent();
      printer.Print(vars, "( request: $RequestName$\n");
      printer.Print(vars, ", metadata: grpc.Metadata | null\n");
      printer.Print(vars, ", options: grpc.CallOptions | null\n");
      printer.Print(vars, ", callback: grpc.requestCallback<$ResponseName$>\n");
      printer.Print(vars, "): void;\n\n");
      printer.Outdent();
    }
  }

  printer.Outdent();
  printer.Print(vars, "}\n\n");

  return true;
}

bool DtsGrpcNodeGenerator::GenerateImports
  ( google::protobuf::io::Printer&           printer
  , const DtsGrpcNodeGeneratorOptions&       options
  , const google::protobuf::FileDescriptor*  file
  , std::string*                             error
  ) const
{

  using google::protobuf::Descriptor;

  std::map<std::string, std::string> vars;
  printer.Print(vars, "import * as grpc from 'grpc';\n");

  auto serviceCount = file->service_count();

  std::map<std::string, std::set<const Descriptor*>> descriptors;

  for(auto i=0; serviceCount > i; ++i) {
    auto service = file->service(i);
    auto methodCount = service->method_count();

    for(auto i=0; methodCount > i; ++i) {
      auto method = service->method(i);
      auto inputType = method->input_type();
      auto outputType = method->output_type();

      auto inputPath = removePathExtname(inputType->file()->name());
      auto outputPath = removePathExtname(outputType->file()->name());

      if(descriptors.find(inputPath) == descriptors.end()) {
        descriptors.insert({inputPath, {}});
      }

      if(descriptors.find(outputPath) == descriptors.end()) {
        descriptors.insert({outputPath, {}});
      }

      descriptors.find(inputPath)->second.insert(inputType);
      descriptors.find(outputPath)->second.insert(outputType);
    }
  }

  for(auto pair : descriptors) {
    auto path = pair.first;
    vars.insert({"ImportPath", "./" + path + "_pb"});

    printer.Print(vars, "import {\n");
    printer.Indent();

    for(auto descriptor : pair.second) {
      vars.insert({"ImportName", descriptor->name()});
      printer.Print(vars, "$ImportName$,\n");
    }

    printer.Outdent();
    printer.Print(vars, "} from '$ImportPath$';\n\n");
  }

  return true;
}

bool DtsGrpcNodeGenerator::Generate
  ( const google::protobuf::FileDescriptor*        file
  , const std::string&                             parameter
  , google::protobuf::compiler::GeneratorContext*  context
  , std::string*                                   error
  ) const
{
  DtsGrpcNodeGeneratorOptions options(parameter);

  if(options.hasError(error)) {
    return false;
  }

  std::unique_ptr<ZeroCopyOutputStream> indexDtsOutput(
    context->Open(removePathExtname(file->name()) + "_grpc_pb.d.ts")
  );
  Printer printer(indexDtsOutput.get(), '$');

  if(!GenerateImports(printer, options, file, error)) {
    return false;
  }

  auto serviceCount = file->service_count();

  for(auto i=0; serviceCount > i; ++i) {
    auto service = file->service(i);

    if(!PrintServiceImplementationInterface(printer, options, service, error)) {
      return false;
    }

    if(!PrintServiceDefinitionInterface(printer, options, service, error)) {
      return false;
    }

    if(!PrintServiceClientClass(printer, options, service, error)) {
      return false;
    }
  }

  return true;
}
