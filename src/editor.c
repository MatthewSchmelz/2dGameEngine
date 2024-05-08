#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Include stdlib.h for malloc and free
#include "simple_logger.h"
#include "simple_json.h"

// Function to modify an entity's stat
void modify_entity_stat(SJson* json, const char* entity_name, const char* stat_name, float new_value) {
    // Get the JSON object representing the entity
    SJson* entity = sj_object_get_value(json, entity_name);
    if (!entity) {
        slog("Entity '%s' not found in JSON.", entity_name);
        return;
    }

    // Delete the existing value of the specified stat
    sj_object_delete_key(entity, stat_name);

    // Create a new JSON object with the updated float value
    SJson* new_value_json = sj_new_float(new_value);
    if (!new_value_json) {
        slog("Failed to create JSON object for updated stat value.");
        return;
    }

    // Insert the updated value of the specified stat
    sj_object_insert(entity, stat_name, new_value_json);

    // Log the modification
    slog("Modified %s's %s to %.2f", entity_name, stat_name, new_value);

    // Save the modified JSON object back to the file
    sj_save(json, "config/enemies.json");
}

// Function to display the menu and get user input
void edit(SJson* json) {
    slog("Welcome to the Editor.\n"
        "In order to select an entity to modify, type its name.\n"
        "If you want to exit, simply type 'exit'.\n"
        "These are the entities available to edit:\n"
        "archmage, barbarian, fighter, hero, mage, ninja, paladin, rogue, shadowmage, tank\n");

    char entity_name[20];
    char stat_name[20];
    float new_value;

    while (1) {
        slog("Enter an entity name: ");
        if (scanf("%19s", entity_name) != 1) {
            slog("Unknown Input!");
            break;
        }

        // Check if the user wants to exit
        if (strcmp(entity_name, "exit") == 0) {
            slog("Exiting!");
            break;
        }

        // Check if the input matches any entity name
        if (sj_object_get_value(json, entity_name) == NULL) {
            slog("Invalid entity name. Please try again.\n");
            continue;
        }

        slog("Selected entity: %s\n", entity_name);

        // Prompt for the stat
        printf("Enter the stat to modify (health, speed, damage): ");
        if (scanf("%19s", stat_name) != 1) {
            printf("Error reading input. Exiting...\n");
            break;
        }

        // Prompt for the new value
        printf("Enter the new value: ");
        if (scanf("%f", &new_value) != 1) {
            printf("Error reading input. Exiting...\n");
            break;
        }

        // Handle the stat input
        modify_entity_stat(json, entity_name, stat_name, new_value);
    }
}

int begin_edit() {
    SJson* json = sj_load("config/enemies.json");
    if (!json) {
        slog("Failed to load enemies json");
        return 1;
    }

    // Call the edit function with the loaded JSON object
    edit(json);

    sj_free(json);
    return 0;
}
