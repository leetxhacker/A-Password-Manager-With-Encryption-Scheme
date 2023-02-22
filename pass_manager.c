#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASSWORDS 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_USERNAME_LENGTH 100
#define MAX_FILENAME_LENGTH 100

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Credentials;

void encrypt(char *password, char *key) {
    // Simple Encryption
    int i, j;
    int password_len = strlen(password);
    int key_len = strlen(key);

    for (i = 0, j = 0; i < password_len; i++, j++) {
        if (j == key_len) {
            j = 0;
        }
        password[i] ^= key[j];
    }
}

void decrypt(char *password, char *key) {
    // Decryption
    encrypt(password, key);
}

void save_credentials(char *filename, Credentials *credentials, int num_credentials, char *key) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    fprintf(f, "%d\n", num_credentials);
    for (int i = 0; i < num_credentials; i++) {
        encrypt(credentials[i].username, key);
        encrypt(credentials[i].password, key);
        fprintf(f, "%s %s\n", credentials[i].username, credentials[i].password);
    }
    fclose(f);
}

int load_credentials(char *filename, Credentials *credentials, char *key) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }

    int num_credentials;
    fscanf(f, "%d", &num_credentials);
    for (int i = 0; i < num_credentials; i++) {
        fscanf(f, "%s %s", credentials[i].username, credentials[i].password);
        decrypt(credentials[i].username, key);
        decrypt(credentials[i].password, key);
    }
    fclose(f);
    return num_credentials;
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    char key[MAX_PASSWORD_LENGTH];
    Credentials credentials[MAX_PASSWORDS];
    int num_credentials = 0;

    printf("Enter the filename to save/load credentials: ");
    scanf("%s", filename);

    printf("Enter the encryption key: ");
    scanf("%s", key);

    num_credentials = load_credentials(filename, credentials, key);
    if (num_credentials == 0) {
        printf("No credentials loaded.\n");
    } else {
        printf("Loaded %d credentials.\n", num_credentials);
    }

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    while (1) {
        printf("Enter a username (or type 'quit' to exit): ");
        scanf("%s", username);
        if (strcmp(username, "quit") == 0) {
            break;
        }

        printf("Enter a password: ");
        scanf("%s", password);

        if (num_credentials < MAX_PASSWORDS) {
            strcpy(credentials[num_credentials].username, username);
            strcpy(credentials[num_credentials].password, password);
            num_credentials++;
            save_credentials(filename, credentials, num_credentials, key);
        } else {
            printf("Error: Maximum number of credentials reached.\n");
        }
    }

    return 0;
}
