set(FLATBUFFERS_FLATC_SCHEMA_EXTRA_ARGS --gen-name-strings --gen-object-api)

set(FLATBUFFERS_SCHEMAS
    error.fbs
    project.fbs)

set(PREFIXED_FLATBUFFERS_SCHEMAS "")
foreach(SCHEMA ${FLATBUFFERS_SCHEMAS})
    list(APPEND PREFIXED_FLATBUFFERS_SCHEMAS ${CMAKE_CURRENT_SOURCE_DIR}/${SCHEMA})
endforeach()

build_flatbuffers(
    "${PREFIXED_FLATBUFFERS_SCHEMAS}"
    ""
    schemas
    ""
    ${CMAKE_BINARY_DIR}/include
    "" "")
