#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the User structure
typedef struct {
    char username[20];
    char password[20];
    int has_voted;
    char voted_for[20];
} User;

// Define the Admin structure
typedef struct {
    char username[20];
    char password[20];
} Admin;

// Define the Candidate structure
typedef struct {
    char name[20];
    int votes;
} Candidate;

// Function Prototypes
void save_candidates(Candidate candidates[], int n);
void load_candidates(Candidate candidates[], int *n);
void save_users(User users[], int n);
void load_users(User users[], int *n);
void save_admin(Admin *admin);
void load_admin(Admin *admin);

int main() {
    // Initialize users and admin
    User users[100];
    Admin admin;
    Candidate candidates[100];

    // Load admin, user and candidate data from files
    load_admin(&admin);
    int user_count = 0, candidate_count = 0;
    load_users(users, &user_count);
    load_candidates(candidates, &candidate_count);

    printf("Welcome to the Polling System!\n");

    // Main program loop
    while (1) {
        // Display the menu
        printf("Choose an option:\n");
        printf("1. User Panel\n");
        printf("2. Admin Panel\n");
        printf("3. Exit\n");

        // Read the user's choice and validate the input
        int choice;
        int input_status;
        do {
            input_status = scanf("%d", &choice);
            while (getchar() != '\n'); // Consume trailing newline characters
            if (input_status != 1) {
                printf("Invalid input! Please enter a number between 1 and 3.\n");
            }
        } while (input_status != 1);

        // Process the user's choice
        switch (choice) {
            case 1: { // User panel
                char username[20], password[20];
                printf("Enter your username: ");
                fgets(username, sizeof(username), stdin);
                size_t len = strlen(username);
                if (len > 0 && username[len - 1] == '\n') {
                    username[--len] = '\0';
                }

                printf("Enter your password: ");
                fgets(password, sizeof(password), stdin);
                len = strlen(password);
                if (len > 0 && password[len - 1] == '\n') {
                    password[--len] = '\0';
                }

                int found = 0;
                for (int i = 0; i < user_count; i++) {
                    if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
                        found = 1;

                        if (users[i].has_voted) {
                            printf("You have already voted for %s!\n", users[i].voted_for);
                            break;
                        } else {
                            printf("Candidates:\n");
                            for (int j = 0; j < candidate_count; j++) {
                                printf("%d. %s\n", j + 1, candidates[j].name);
                            }

                            int vote;
                            printf("Enter the number of the candidate you want to vote for: ");
                            scanf("%d", &vote);

                            if (vote >= 1 && vote <= candidate_count) {
                                candidates[vote - 1].votes++;
                                users[i].has_voted = 1;
                                strcpy(users[i].voted_for, candidates[vote - 1].name);
                                save_users(users, user_count);
                                save_candidates(candidates, candidate_count);
                                printf("Your vote has been counted. Thank you!\n");
                            } else {
                                printf("Invalid candidate number.\n");
                            }
                            break;
                        }
                    }
                }

                if (!found) {
                    printf("Invalid user credentials!\n");
                }
                break;
            }
            case 2: { // Admin panel
                char username[20], password[20];
                printf("Enter admin username: ");
                fgets(username, sizeof(username), stdin);
                size_t len = strlen(username);
                if (len > 0 && username[len - 1] == '\n') {
                    username[--len] = '\0';
                }

                printf("Enter admin password: ");
                fgets(password, sizeof(password), stdin);
                len = strlen(password);
                if (len > 0 && password[len - 1] == '\n') {
                    password[--len] = '\0';
                }

                if (strcmp(username, admin.username) == 0 && strcmp(password, admin.password) == 0) {
                    int admin_choice;
                    do {
                        printf("Welcome Admin!\n");
                        printf("1. Declare result\n");
                        printf("2. Fix user or candidate issues\n");
                        printf("3. Change admin password\n");
                        printf("4. Exit admin panel\n");

                        scanf("%d", &admin_choice);

                        if (admin_choice == 1) {
                            printf("Election results:\n");
                            for (int i = 0; i < candidate_count; i++) {
                                printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
                            }
                        } else if (admin_choice == 2) {
                            printf("1. Add new user\n");
                            printf("2. Add new candidate\n");
                            printf("3. Reset user voting status\n");
                            printf("4. Go back\n");

                            int fix_choice;
                            scanf("%d", &fix_choice);

                            if (fix_choice == 1) {
                                printf("Enter new user's username: ");
                                scanf("%s", users[user_count].username);
                                printf("Enter new user's password: ");
                                scanf("%s", users[user_count].password);
                                users[user_count].has_voted = 0;
                                strcpy(users[user_count].voted_for, "n/a");
                                user_count++;
                                save_users(users, user_count);
                                printf("User added successfully.\n");
                            } else if (fix_choice == 2) {
                                printf("Enter new candidate's name: ");
                                scanf("%s", candidates[candidate_count].name);
                                candidates[candidate_count].votes = 0;
                                candidate_count++;
                                save_candidates(candidates, candidate_count);
                                printf("Candidate added successfully.\n");
                            } else if (fix_choice == 3) {
                                char reset_username[20];
                                printf("Enter the username of the user whose vote you want to reset: ");
                                scanf("%s", reset_username);

                                int user_found = 0;
                                for (int i = 0; i < user_count; i++) {
                                    if (strcmp(reset_username, users[i].username) == 0) {
                                        user_found = 1;
                                        if (users[i].has_voted) {
                                            users[i].has_voted = 0;
                                            strcpy(users[i].voted_for, "n/a"); // Reset the voted_for field
                                            save_users(users, user_count);
                                            printf("User's vote has been reset.\n");
                                        } else {
                                            printf("This user has not voted yet.\n");
                                        }
                                        break;
                                    }
                                }
                                if (!user_found) {
                                    printf("User not found.\n");
                                }
                            } else if (fix_choice == 4) {
                                // Go back to the admin panel
                            } else {
                                printf("Invalid choice.\n");
                            }
                        } else if (admin_choice == 3) {
                            printf("Enter your new password: ");
                            scanf("%s", admin.password);
                            save_admin(&admin);
                            printf("Admin password updated successfully.\n");
                        } else if (admin_choice == 4) {
                            printf("Exiting admin panel.\n");
                            break;
                        } else {
                            printf("Invalid choice.\n");
                        }
                    } while (admin_choice != 4);
                } else {
                    printf("Invalid admin credentials!\n");
                }
                break;
            }
            case 3: // Exit
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }

    return 0;
}
// Function to save the admin to the "admin.txt" file
void save_admin(Admin *admin) {
    FILE *file = fopen("admin.txt", "w");
    if (file == NULL) {
        printf("Error opening admin file.\n");
        return;
    }

    fprintf(file, "%s %s\n", admin->username, admin->password);
    fclose(file);
}

// Function to load the admin from the "admin.txt" file
void load_admin(Admin *admin) {
    FILE *file = fopen("admin.txt", "r");
    if (file == NULL) {
        printf("Error opening admin file.\n");
        printf("Initializing default admin credentials.\n");
        strcpy(admin->username, "admin");
        strcpy(admin->password, "password");
        save_admin(admin);
        return;
    }

    fscanf(file, "%s %s", admin->username, admin->password);
    fclose(file);
}

// Function to save candidates to the "candidates.txt" file
void save_candidates(Candidate candidates[], int n) {
    FILE *file = fopen("candidates.txt", "w");
    if (file == NULL) {
        printf("Error opening candidates file.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%s %d\n", candidates[i].name, candidates[i].votes);
    }

    fclose(file);
}

// Function to load candidates from the "candidates.txt" file
void load_candidates(Candidate candidates[], int *n) {
    FILE *file = fopen("candidates.txt", "r");
    if (file == NULL) {
        printf("Error opening candidates file.\n");
        return;
    }

    *n = 0;
    while (fscanf(file, "%s %d", candidates[*n].name, &candidates[*n].votes) != EOF) {
        (*n)++;
    }

    fclose(file);
}

// Function to save users to the "users.txt" file
void save_users(User users[], int n) {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Error opening users file.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%s %s %d %s\n", users[i].username, users[i].password, users[i].has_voted, users[i].voted_for);
    }

    fclose(file);
}

// Function to load users from the "users.txt" file
void load_users(User users[], int *n) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening users file.\n");
        return;
    }

    *n = 0;
    while (fscanf(file, "%s %s %d %s", users[*n].username, users[*n].password, &users[*n].has_voted, users[*n].voted_for) != EOF) {
        (*n)++;
    }

    fclose(file);
}


