// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Diagnostics.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Diagnostics_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Diagnostics_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Diagnostics_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Diagnostics_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Diagnostics_2eproto;
class diagnostics_v1;
struct diagnostics_v1DefaultTypeInternal;
extern diagnostics_v1DefaultTypeInternal _diagnostics_v1_default_instance_;
class diagnostics_v1_DiagnosticsEntry_DoNotUse;
struct diagnostics_v1_DiagnosticsEntry_DoNotUseDefaultTypeInternal;
extern diagnostics_v1_DiagnosticsEntry_DoNotUseDefaultTypeInternal _diagnostics_v1_DiagnosticsEntry_DoNotUse_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::diagnostics_v1* Arena::CreateMaybeMessage<::diagnostics_v1>(Arena*);
template<> ::diagnostics_v1_DiagnosticsEntry_DoNotUse* Arena::CreateMaybeMessage<::diagnostics_v1_DiagnosticsEntry_DoNotUse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class diagnostics_v1_DiagnosticsEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<diagnostics_v1_DiagnosticsEntry_DoNotUse, 
    std::string, std::string,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING> {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<diagnostics_v1_DiagnosticsEntry_DoNotUse, 
    std::string, std::string,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING> SuperType;
  diagnostics_v1_DiagnosticsEntry_DoNotUse();
  explicit constexpr diagnostics_v1_DiagnosticsEntry_DoNotUse(
      ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);
  explicit diagnostics_v1_DiagnosticsEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const diagnostics_v1_DiagnosticsEntry_DoNotUse& other);
  static const diagnostics_v1_DiagnosticsEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const diagnostics_v1_DiagnosticsEntry_DoNotUse*>(&_diagnostics_v1_DiagnosticsEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(std::string* s) {
    return ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(s->data(), static_cast<int>(s->size()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "diagnostics_v1.DiagnosticsEntry.key");
 }
  static bool ValidateValue(std::string* s) {
    return ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(s->data(), static_cast<int>(s->size()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "diagnostics_v1.DiagnosticsEntry.value");
 }
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
};

// -------------------------------------------------------------------

class diagnostics_v1 final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:diagnostics_v1) */ {
 public:
  inline diagnostics_v1() : diagnostics_v1(nullptr) {}
  ~diagnostics_v1() override;
  explicit constexpr diagnostics_v1(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  diagnostics_v1(const diagnostics_v1& from);
  diagnostics_v1(diagnostics_v1&& from) noexcept
    : diagnostics_v1() {
    *this = ::std::move(from);
  }

  inline diagnostics_v1& operator=(const diagnostics_v1& from) {
    CopyFrom(from);
    return *this;
  }
  inline diagnostics_v1& operator=(diagnostics_v1&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const diagnostics_v1& default_instance() {
    return *internal_default_instance();
  }
  static inline const diagnostics_v1* internal_default_instance() {
    return reinterpret_cast<const diagnostics_v1*>(
               &_diagnostics_v1_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(diagnostics_v1& a, diagnostics_v1& b) {
    a.Swap(&b);
  }
  inline void Swap(diagnostics_v1* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(diagnostics_v1* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline diagnostics_v1* New() const final {
    return new diagnostics_v1();
  }

  diagnostics_v1* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<diagnostics_v1>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const diagnostics_v1& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const diagnostics_v1& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(diagnostics_v1* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "diagnostics_v1";
  }
  protected:
  explicit diagnostics_v1(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  enum : int {
    kDiagnosticsFieldNumber = 1,
  };
  // map<string, string> diagnostics = 1;
  int diagnostics_size() const;
  private:
  int _internal_diagnostics_size() const;
  public:
  void clear_diagnostics();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
      _internal_diagnostics() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
      _internal_mutable_diagnostics();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
      diagnostics() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
      mutable_diagnostics();

  // @@protoc_insertion_point(class_scope:diagnostics_v1)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::MapField<
      diagnostics_v1_DiagnosticsEntry_DoNotUse,
      std::string, std::string,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING> diagnostics_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Diagnostics_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// diagnostics_v1

// map<string, string> diagnostics = 1;
inline int diagnostics_v1::_internal_diagnostics_size() const {
  return diagnostics_.size();
}
inline int diagnostics_v1::diagnostics_size() const {
  return _internal_diagnostics_size();
}
inline void diagnostics_v1::clear_diagnostics() {
  diagnostics_.Clear();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
diagnostics_v1::_internal_diagnostics() const {
  return diagnostics_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
diagnostics_v1::diagnostics() const {
  // @@protoc_insertion_point(field_map:diagnostics_v1.diagnostics)
  return _internal_diagnostics();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
diagnostics_v1::_internal_mutable_diagnostics() {
  return diagnostics_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
diagnostics_v1::mutable_diagnostics() {
  // @@protoc_insertion_point(field_mutable_map:diagnostics_v1.diagnostics)
  return _internal_mutable_diagnostics();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Diagnostics_2eproto