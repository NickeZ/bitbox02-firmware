"""
@generated by mypy-protobuf.  Do not edit manually!
isort:skip_file
Copyright 2019 Shift Cryptosecurity AG

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import builtins
import google.protobuf.descriptor
import google.protobuf.internal.enum_type_wrapper
import google.protobuf.message
import sys
import typing

if sys.version_info >= (3, 10):
    import typing as typing_extensions
else:
    import typing_extensions

DESCRIPTOR: google.protobuf.descriptor.FileDescriptor

@typing.final
class RebootRequest(google.protobuf.message.Message):
    DESCRIPTOR: google.protobuf.descriptor.Descriptor

    class _Purpose:
        ValueType = typing.NewType("ValueType", builtins.int)
        V: typing_extensions.TypeAlias = ValueType

    class _PurposeEnumTypeWrapper(google.protobuf.internal.enum_type_wrapper._EnumTypeWrapper[RebootRequest._Purpose.ValueType], builtins.type):
        DESCRIPTOR: google.protobuf.descriptor.EnumDescriptor
        UPGRADE: RebootRequest._Purpose.ValueType  # 0
        SETTINGS: RebootRequest._Purpose.ValueType  # 1

    class Purpose(_Purpose, metaclass=_PurposeEnumTypeWrapper): ...
    UPGRADE: RebootRequest.Purpose.ValueType  # 0
    SETTINGS: RebootRequest.Purpose.ValueType  # 1

    PURPOSE_FIELD_NUMBER: builtins.int
    purpose: global___RebootRequest.Purpose.ValueType
    def __init__(
        self,
        *,
        purpose: global___RebootRequest.Purpose.ValueType = ...,
    ) -> None: ...
    def ClearField(self, field_name: typing.Literal["purpose", b"purpose"]) -> None: ...

global___RebootRequest = RebootRequest
