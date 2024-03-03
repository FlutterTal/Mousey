/******************************************************************************/
/* Copyright (c) 2024 Taliesin Perscilla "FlutterTal" Ambroise                */
/******************************************************************************/
/* This software is provided ‘as-is’, without any express or implied          */
/* warranty. In no event will the authors be held liable for any damages      */
/* arising from the use of this software.                                     */
/*                                                                            */
/* Permission is granted to anyone to use this software for any purpose,      */
/* including commercial applications, and to alter it and redistribute it     */
/* freely, subject to the following restrictions:                             */
/*                                                                            */
/* 1. The origin of this software must not be misrepresented; you must not    */
/* claim that you wrote the original software. If you use this software       */
/* in a product, an acknowledgment in the product documentation would be      */
/* appreciated but is not required.                                           */
/*                                                                            */
/* 2. Altered source versions must be plainly marked as such, and must not be */
/* misrepresented as being the original software.                             */
/*                                                                            */
/* 3. This notice may not be removed or altered from any source               */
/* distribution.                                                              */
/******************************************************************************/

#include "yaml_emitter.h"
#include <string.h>

YAMLEmitter::YAMLEmitter() {
    file = fopen("project.yaml", "wb");
    if(!yaml_emitter_initialize(&emitter)) {
        fprintf(stderr, "Failed to initialize the emitter\n");
        return;
    }

    yaml_emitter_set_output_file(&emitter, file);
    yaml_emitter_set_canonical(&emitter, 0);
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    yaml_emitter_emit(&emitter, &event);
    yaml_document_start_event_initialize(&event, nullptr, nullptr, nullptr, 0);
    yaml_emitter_emit(&emitter, &event);
    yaml_mapping_start_event_initialize(&event, nullptr, nullptr, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);
}

YAMLEmitter::~YAMLEmitter() {
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);
    yaml_document_end_event_initialize(&event, 0);
    yaml_emitter_emit(&emitter, &event);
    yaml_stream_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);
    yaml_event_delete(&event);
    yaml_emitter_delete(&emitter);
    fclose(file);
}

void YAMLEmitter::new_block(const char* name) {
    yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)name, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);
    yaml_mapping_start_event_initialize(&event, nullptr, nullptr, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);
}

void YAMLEmitter::end_block() {
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);
}

void YAMLEmitter::new_scalar(const char* key, const char* value) {
    yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)key, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);
    yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)value, -1, 1, 1, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);
}