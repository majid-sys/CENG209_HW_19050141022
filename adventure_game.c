/*
Name: Majed Mojahed Mohammed Younis
Id: 19050141022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Custom strdup implementation for compatibility
char *strdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

// Define Structures
typedef struct {
    int health;
    int strength;
    int inventory_limit;
    char **inventory;
    int items_count;
} Player;

typedef struct {
    int health;
    int attack_power;
    char *type;
} Creature;

typedef struct Room {
    char *description;
    struct Room *north;
    struct Room *south;
    struct Room *west;
    struct Room *east;
    char **objects;
    int object_count;
    Creature *enemy;
} Room;

// Function Prototypes
void create_player(Player *player);
void create_room(Room *room, const char *details);
void release_room(Room *room);
void relocate_player(Room **current_room, const char *direction);
void observe_room(Room *current_room);
void gather_item(Player *player, Room *current_room, const char *item_name);
void engage_enemy(Player *player, Creature *enemy);
void store_game(Player *player, Room *current_room, const char *filepath);
void retrieve_game(Player *player, Room **current_room, const char *filepath);
void list_saved_games();

// Function Implementations
void create_player(Player *player) {
    player->health = 100;
    player->strength = 10;
    player->inventory_limit = 5;
    player->inventory = malloc(sizeof(char *) * player->inventory_limit);
    player->items_count = 0;
}

void create_room(Room *room, const char *details) {
    room->description = strdup(details);
    room->north = room->south = room->west = room->east = NULL;
    room->objects = NULL;
    room->object_count = 0;
    room->enemy = NULL;
}

void release_room(Room *room) {
    free(room->description);
    for (int i = 0; i < room->object_count; i++) {
        free(room->objects[i]);
    }
    free(room->objects);
    if (room->enemy) {
        free(room->enemy->type);
        free(room->enemy);
    }
}

void relocate_player(Room **current_room, const char *direction) {
    Room *next = NULL;
    if (strcmp(direction, "north") == 0) {
        next = (*current_room)->north;
    } else if (strcmp(direction, "south") == 0) {
        next = (*current_room)->south;
    } else if (strcmp(direction, "west") == 0) {
        next = (*current_room)->west;
    } else if (strcmp(direction, "east") == 0) {
        next = (*current_room)->east;
    }

    if (next) {
        *current_room = next;
        printf("You head %s.\n", direction);
    } else {
        printf("There's no path in that direction.\n");
    }
}

void observe_room(Room *current_room) {
    printf("Location details: %s\n", current_room->description);
    if (current_room->object_count > 0) {
        printf("Items available:\n");
        for (int i = 0; i < current_room->object_count; i++) {
            printf("- %s\n", current_room->objects[i]);
        }
    }
    if (current_room->enemy) {
        printf("An enemy is here: %s\n", current_room->enemy->type);
    }
}

void gather_item(Player *player, Room *current_room, const char *item_name) {
    for (int i = 0; i < current_room->object_count; i++) {
        if (strcmp(current_room->objects[i], item_name) == 0) {
            if (player->items_count < player->inventory_limit) {
                player->inventory[player->items_count++] = strdup(item_name);
                printf("You picked up %s.\n", item_name);

                for (int j = i; j < current_room->object_count - 1; j++) {
                    current_room->objects[j] = current_room->objects[j + 1];
                }
                current_room->object_count--;
                return;
            } else {
                printf("Your inventory is full.\n");
                return;
            }
        }
    }
    printf("No such item in this room.\n");
}

void engage_enemy(Player *player, Creature *enemy) {
    if (!enemy) {
        printf("There's no enemy here to fight.\n");
        return;
    }

    while (player->health > 0 && enemy->health > 0) {
        enemy->health -= player->strength;
        printf("You attacked the %s. It has %d health remaining.\n", enemy->type, enemy->health);

        if (enemy->health > 0) {
            player->health -= enemy->attack_power;
            printf("The %s hit you. You have %d health left.\n", enemy->type, player->health);
        }
    }

    if (player->health > 0) {
        printf("You defeated the %s!\n", enemy->type);
        free(enemy->type);
        free(enemy);
    } else {
        printf("You were slain by the %s. Game over.\n", enemy->type);
    }
}

void store_game(Player *player, Room *current_room, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Unable to save game");
        return;
    }

    fprintf(file, "%d %d %d\n", player->health, player->strength, player->items_count);
    for (int i = 0; i < player->items_count; i++) {
        fprintf(file, "%s\n", player->inventory[i]);
    }

    fprintf(file, "%s\n", current_room->description);
    fclose(file);
    printf("Game saved at %s.\n", filepath);
}

void retrieve_game(Player *player, Room **current_room, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Unable to load game");
        return;
    }

    fscanf(file, "%d %d %d\n", &player->health, &player->strength, &player->items_count);
    for (int i = 0; i < player->items_count; i++) {
        char buffer[256];
        fscanf(file, "%s\n", buffer);
        player->inventory[i] = strdup(buffer);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    buffer[strcspn(buffer, "\n")] = 0;
    (*current_room)->description = strdup(buffer);

    fclose(file);
    printf("Game loaded from %s.\n", filepath);
}

void list_saved_games() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (!dir) {
        perror("Unable to open directory");
        return;
    }

    printf("Saved games:\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt")) {
            printf("- %s\n", entry->d_name);
        }
    }
    closedir(dir);
}

int main() {
    Player player;
    Room room1, room2;

    create_player(&player);
    create_room(&room1, "You find yourself in a shadowy cavern.");
    create_room(&room2, "You have entered a sunlit clearing.");

    room1.east = &room2;
    room2.west = &room1;

    Room *current_room = &room1;

    char command[256];
    printf("Welcome to the Adventure Game!\n");

    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "list") == 0) {
            list_saved_games();
        } else if (strncmp(command, "move", 4) == 0) {
            relocate_player(&current_room, command + 5);
        } else if (strcmp(command, "look") == 0) {
            observe_room(current_room);
        } else if (strncmp(command, "pickup", 6) == 0) {
            gather_item(&player, current_room, command + 7);
        } else if (strcmp(command, "inventory") == 0) {
            for (int i = 0; i < player.items_count; i++) {
                printf("- %s\n", player.inventory[i]);
            }
        } else if (strncmp(command, "save", 4) == 0) {
            store_game(&player, current_room, command + 5);
        } else if (strncmp(command, "load", 4) == 0) {
            retrieve_game(&player, &current_room, command + 5);
        } else {
            printf("Invalid command.\n");
        }
    }

    release_room(&room1);
    release_room(&room2);
    free(player.inventory);

    printf("Thanks for playing the game!\n");
    return 0;
}
