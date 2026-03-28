#include "gnome_shared.hpp"

namespace system_theme_pp {

    // Helper: check if a GSettings schema exists
    bool schemaExists(const char* schemaId) {
        GSettingsSchemaSource* source = g_settings_schema_source_get_default();
        GSettingsSchema*       schema = g_settings_schema_source_lookup(source, schemaId, TRUE);
        if(schema) {
            g_settings_schema_unref(schema);
            return true;
        }
        return false;
    }
}  // namespace system_theme_pp