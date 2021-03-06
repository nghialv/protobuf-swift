// Protocol Buffers for Swift
//
// Copyright 2014 Alexey Khohklov(AlexeyXo).
// Copyright 2008 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "swift_extension.h"

#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/io/printer.h>

#include "swift_extension.h"
#include "swift_helpers.h"

namespace google { namespace protobuf { namespace compiler { namespace swift {
    
    ExtensionGenerator::ExtensionGenerator(string classname, const FieldDescriptor* descriptor)
    : classname_(classname),
    descriptor_(descriptor) {
    }
    
    
    ExtensionGenerator::~ExtensionGenerator() {
    }
    
    
    void ExtensionGenerator::GenerateFieldsGetterSource(io::Printer* printer, string rootclassname) {
        map<string, string> vars;
        vars["name"] = UnderscoresToCamelCase(descriptor_);
        ////
        vars["containing_type"] = classname_;
        
        ////
        vars["root_name"] = rootclassname;
        
        SwiftType swift_type = GetSwiftType(descriptor_);
        string singular_type;
        switch (swift_type) {
            case SWIFTTYPE_MESSAGE:
                vars["type"] = ClassName(descriptor_->message_type());
                break;
            default:
                vars["type"] = BoxedPrimitiveTypeName(swift_type);
                break;
        }
        
        vars["extended_type"] = ClassName(descriptor_->containing_type());
        vars["acontrol"] = GetAccessControlType(descriptor_->file());
        
        printer->Print(vars,"$acontrol$ var $containing_type$$name$:ConcreateExtensionField {\n"
                       "   get {\n"
                       "       return $root_name$.sharedInstance.$containing_type$$name$Static\n"
                       "   }\n"
                       "}\n");
    }
    
    void ExtensionGenerator::GenerateFieldsSource(io::Printer* printer) {
        map<string, string> vars;
        vars["name"] = UnderscoresToCamelCase(descriptor_);
        vars["containing_type"] = classname_;
        
        SwiftType swift_type = GetSwiftType(descriptor_);
        string singular_type;
        switch (swift_type) {
            case SWIFTTYPE_MESSAGE:
                vars["type"] = ClassName(descriptor_->message_type());
                break;
            default:
                vars["type"] = BoxedPrimitiveTypeName(swift_type);
                break;
        }
        
        vars["extended_type"] = ClassName(descriptor_->containing_type());
        vars["acontrol"] = GetAccessControlType(descriptor_->file());
        printer->Print(vars,
                       "var $containing_type$$name$Static:ConcreateExtensionField\n");
    }
    
    void ExtensionGenerator::GenerateMembersSourceExtensions(io::Printer* printer, string fileClass) {
        map<string, string> vars;
        vars["name"] = UnderscoresToCamelCase(descriptor_);
        vars["containing_type"] = classname_;
        vars["rootclass_type"] = fileClass;
        vars["acontrol"] = GetAccessControlType(descriptor_->file());
        printer->Print(vars,
                       "$acontrol$ static func $name$() -> ConcreateExtensionField {\n"
                       "     return $rootclass_type$.sharedInstance.$containing_type$$name$Static\n"
                       "}\n");
    }
    
    void ExtensionGenerator::GenerateMembersSource(io::Printer* printer) {
        map<string, string> vars;
        vars["name"] = UnderscoresToCamelCase(descriptor_);
        vars["containing_type"] = classname_;
        vars["acontrol"] = GetAccessControlType(descriptor_->file());
        printer->Print(vars,
                       "$acontrol$ class func $name$() -> ConcreateExtensionField {\n"
                       "     return $containing_type$$name$\n"
                       "}\n");
    }
    
    void ExtensionGenerator::GenerateInitializationSource(io::Printer* printer) {
        map<string, string> vars;
        vars["name"] = UnderscoresToCamelCase(descriptor_);
        vars["containing_type"] = classname_;
        vars["extended_type"] = ClassNameReturedType(descriptor_->containing_type());
        vars["number"] = SimpleItoa(descriptor_->number());
        
        const bool isPacked = descriptor_->options().packed();
        vars["is_repeated"] = descriptor_->is_repeated() ? "true" : "false";
        vars["is_packed"] = isPacked ? "true" : "false";
        vars["is_wire_format"] = descriptor_->containing_type()->options().message_set_wire_format() ? "true" : "false";
        
        SwiftType swift_type = GetSwiftType(descriptor_);
        string singular_type;
        switch (swift_type) {
            case SWIFTTYPE_MESSAGE:
                vars["type"] = ClassNameReturedType(descriptor_->message_type());
                break;
            default:
                vars["type"] = BoxedPrimitiveTypeName(swift_type);
                break;
        }
        
        switch (descriptor_->type()) {
            case FieldDescriptor::TYPE_INT32:
                vars["extension_type"] = "ExtensionType.ExtensionTypeInt32";
                break;
            case FieldDescriptor::TYPE_UINT32:
                vars["extension_type"] = "ExtensionType.ExtensionTypeUInt32";
                break;
            case FieldDescriptor::TYPE_SINT32:
                vars["extension_type"] = "ExtensionType.ExtensionTypeSInt32";
                break;
            case FieldDescriptor::TYPE_FIXED32:
                vars["extension_type"] = "ExtensionType.ExtensionTypeFixed32";
                break;
            case FieldDescriptor::TYPE_SFIXED32:
                vars["extension_type"] = "ExtensionType.ExtensionTypeSFixed32";
                break;
            case FieldDescriptor::TYPE_INT64:
                vars["extension_type"] = "ExtensionType.ExtensionTypeInt64";
                break;
            case FieldDescriptor::TYPE_UINT64:
                vars["extension_type"] = "ExtensionType.ExtensionTypeUInt64";
                break;
            case FieldDescriptor::TYPE_SINT64:
                vars["extension_type"] = "ExtensionType.ExtensionTypeSInt64";
                break;
            case FieldDescriptor::TYPE_FIXED64:
                vars["extension_type"] = "ExtensionType.ExtensionTypeFixed64";
                break;
            case FieldDescriptor::TYPE_SFIXED64:
                vars["extension_type"] = "ExtensionType.ExtensionTypeSFixed64";
                break;
            case FieldDescriptor::TYPE_FLOAT:
                vars["extension_type"] = "ExtensionType.ExtensionTypeFloat";
                break;
            case FieldDescriptor::TYPE_DOUBLE:
                vars["extension_type"] = "ExtensionType.ExtensionTypeDouble";
                break;
            case FieldDescriptor::TYPE_BOOL:
                vars["extension_type"] = "ExtensionType.ExtensionTypeBool";
                break;
            case FieldDescriptor::TYPE_STRING:
                vars["extension_type"] = "ExtensionType.ExtensionTypeString";
                break;
            case FieldDescriptor::TYPE_BYTES:
                vars["extension_type"] = "ExtensionType.ExtensionTypeBytes";
                break;
            case FieldDescriptor::TYPE_MESSAGE:
                vars["extension_type"] = "ExtensionType.ExtensionTypeMessage";
                break;
            case FieldDescriptor::TYPE_ENUM:
                vars["extension_type"] = "ExtensionType.ExtensionTypeEnum";
                break;
            case FieldDescriptor::TYPE_GROUP:
                vars["extension_type"] = "ExtensionType.ExtensionTypeGroup";
                break;
        }
        
        if(descriptor_->is_repeated())
        {
            SwiftType swift_type = GetSwiftType(descriptor_);
            if (swift_type == SWIFTTYPE_MESSAGE)
            {
                vars["default"] = string("Array<GeneratedMessage>()");
            }
            else
            {
                vars["default"] = string("Array<") + vars["type"] + string(">()");
            }
            
        }
        
        else if (descriptor_->type() == FieldDescriptor::TYPE_ENUM)
        {
            vars["default"] =  ClassNameReturedType(descriptor_->enum_type()) + "." + EnumValueName(descriptor_->default_value_enum()) + ".rawValue";
        }
        else
        {
            vars["default"] =  DefaultValue(descriptor_);
        }
        
        printer->Print(vars,"$containing_type$$name$Static = ConcreateExtensionField(type:$extension_type$, extendedClass:$extended_type$.self, fieldNumber: $number$, defaultValue:$default$, messageOrGroupClass:$type$.self, isRepeated:$is_repeated$, isPacked:$is_packed$, isMessageSetWireFormat:$is_wire_format$)\n");
    }
    
    void ExtensionGenerator::GenerateRegistrationSource(io::Printer* printer) {
        printer->Print(
                       "registry.addExtension($scope$$name$Static)\n",
                       "scope", classname_,
                       "name", UnderscoresToCamelCase(descriptor_));
    }
}  // namespace swift
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
