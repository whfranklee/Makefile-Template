// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WiFiConnect.proto

#include "WiFiConnect.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
constexpr wifi_connect_v1::wifi_connect_v1(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : service_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , password_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string){}
struct wifi_connect_v1DefaultTypeInternal {
  constexpr wifi_connect_v1DefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~wifi_connect_v1DefaultTypeInternal() {}
  union {
    wifi_connect_v1 _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT wifi_connect_v1DefaultTypeInternal _wifi_connect_v1_default_instance_;
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_WiFiConnect_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_WiFiConnect_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_WiFiConnect_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_WiFiConnect_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::wifi_connect_v1, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::wifi_connect_v1, service_),
  PROTOBUF_FIELD_OFFSET(::wifi_connect_v1, password_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::wifi_connect_v1)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_wifi_connect_v1_default_instance_),
};

const char descriptor_table_protodef_WiFiConnect_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\021WiFiConnect.proto\"4\n\017wifi_connect_v1\022\017"
  "\n\007service\030\001 \001(\t\022\020\n\010password\030\002 \001(\tb\006proto"
  "3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_WiFiConnect_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_WiFiConnect_2eproto = {
  false, false, 81, descriptor_table_protodef_WiFiConnect_2eproto, "WiFiConnect.proto", 
  &descriptor_table_WiFiConnect_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_WiFiConnect_2eproto::offsets,
  file_level_metadata_WiFiConnect_2eproto, file_level_enum_descriptors_WiFiConnect_2eproto, file_level_service_descriptors_WiFiConnect_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_WiFiConnect_2eproto_getter() {
  return &descriptor_table_WiFiConnect_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_WiFiConnect_2eproto(&descriptor_table_WiFiConnect_2eproto);

// ===================================================================

class wifi_connect_v1::_Internal {
 public:
};

wifi_connect_v1::wifi_connect_v1(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:wifi_connect_v1)
}
wifi_connect_v1::wifi_connect_v1(const wifi_connect_v1& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  service_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_service().empty()) {
    service_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_service(), 
      GetArenaForAllocation());
  }
  password_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_password().empty()) {
    password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_password(), 
      GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:wifi_connect_v1)
}

inline void wifi_connect_v1::SharedCtor() {
service_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
password_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

wifi_connect_v1::~wifi_connect_v1() {
  // @@protoc_insertion_point(destructor:wifi_connect_v1)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void wifi_connect_v1::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  service_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  password_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void wifi_connect_v1::ArenaDtor(void* object) {
  wifi_connect_v1* _this = reinterpret_cast< wifi_connect_v1* >(object);
  (void)_this;
}
void wifi_connect_v1::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void wifi_connect_v1::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void wifi_connect_v1::Clear() {
// @@protoc_insertion_point(message_clear_start:wifi_connect_v1)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  service_.ClearToEmpty();
  password_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* wifi_connect_v1::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string service = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_service();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "wifi_connect_v1.service"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string password = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_password();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "wifi_connect_v1.password"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* wifi_connect_v1::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:wifi_connect_v1)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string service = 1;
  if (!this->_internal_service().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_service().data(), static_cast<int>(this->_internal_service().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "wifi_connect_v1.service");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_service(), target);
  }

  // string password = 2;
  if (!this->_internal_password().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_password().data(), static_cast<int>(this->_internal_password().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "wifi_connect_v1.password");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_password(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:wifi_connect_v1)
  return target;
}

size_t wifi_connect_v1::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:wifi_connect_v1)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string service = 1;
  if (!this->_internal_service().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_service());
  }

  // string password = 2;
  if (!this->_internal_password().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_password());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData wifi_connect_v1::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    wifi_connect_v1::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*wifi_connect_v1::GetClassData() const { return &_class_data_; }

void wifi_connect_v1::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<wifi_connect_v1 *>(to)->MergeFrom(
      static_cast<const wifi_connect_v1 &>(from));
}


void wifi_connect_v1::MergeFrom(const wifi_connect_v1& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:wifi_connect_v1)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_service().empty()) {
    _internal_set_service(from._internal_service());
  }
  if (!from._internal_password().empty()) {
    _internal_set_password(from._internal_password());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void wifi_connect_v1::CopyFrom(const wifi_connect_v1& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:wifi_connect_v1)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool wifi_connect_v1::IsInitialized() const {
  return true;
}

void wifi_connect_v1::InternalSwap(wifi_connect_v1* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &service_, GetArenaForAllocation(),
      &other->service_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &password_, GetArenaForAllocation(),
      &other->password_, other->GetArenaForAllocation()
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata wifi_connect_v1::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_WiFiConnect_2eproto_getter, &descriptor_table_WiFiConnect_2eproto_once,
      file_level_metadata_WiFiConnect_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::wifi_connect_v1* Arena::CreateMaybeMessage< ::wifi_connect_v1 >(Arena* arena) {
  return Arena::CreateMessageInternal< ::wifi_connect_v1 >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
