#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000
#define MAX_COMMAND_LENGTH 1000
#define MAX_NAME_LENGTH 1000

#define COLOR_RESET "\e[0m"
#define CYAN "\e[0;36m"
#define YELLOW "\e[0;33m"
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define BLU "\e[0;34m"
#define PURPLE "\e[0;35m"

int init(int argc, char *const argv[]);
int configs(int argc, char *const argv[]);

struct dirent *search_in_directory(char *search_file, char *address);
struct dirent *HEAD_finder(char *address, int n);

int check_for_add(char *filename, char *address);
int add(int argc, char *const argv[]);

int reset(int argc, char *const argv[]);

int commit(int argc, char *const argv[]);
int set(int argc, char *const argv[]);
void find_shortcut(char *shortcut_name, int *flag_not_exist);
int replace(int argc, char *const argv[]);
int remove_func(int argc, char *const argv[]);

int branch(int argc, char *const argv[]);

int log_func(int argc, char *const argv[]);

int checkout(int argc, char *const argv[]);

int status(int argc, char *const argv[]);

int revert(int argc, char *const argv[]);

int tag(int argc, char *const argv[]);

int grep(int argc, char *const argv[]);

int diff(int argc, char *const argv[]);

int pre_commit(int argc, char *const argv[]);

int init(int argc, char *const argv[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        return 1;
    }
    FILE *file_name = fopen("/Users/parmis/Desktop/project/FOP/config_global/config_user_name", "r");
    FILE *file_email = fopen("/Users/parmis/Desktop/project/FOP/config_global/config_user_email", "r");
    if ((file_name == NULL) && (file_email == NULL))
    {
        fprintf(stderr, "Error no user.name exists!\n");
        fprintf(stderr, "Error no user.email exists!");
        return 1;
    }
    else if ((file_name == NULL) && (file_email != NULL))
    {
        fprintf(stderr, "Error no user.name exists!");
        return 1;
    }
    else if ((file_name != NULL) && (file_email == NULL))
    {
        fprintf(stderr, "Error no user.email exists!");
        return 1;
    }
    else
    {
        char tmp_cwd[1024];
        bool exists = false;
        struct dirent *entry;
        do
        {
            DIR *dir = opendir(".");
            if (dir == NULL)
            {
                fprintf(stderr, "Error opening current directory");
                return 1;
            }
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".changiz") == 0)
                {
                    exists = true;
                }
            }
            closedir(dir);

            if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            {
                return 1;
            }

            if (strcmp(tmp_cwd, "/") != 0)
            {
                if (chdir("..") != 0)
                {
                    return 1;
                }
            }

        } while (strcmp(tmp_cwd, "/") != 0);

        if (chdir(cwd) != 0)
        {
            return 1;
        }

        if (!exists)
        {
            if (mkdir(".changiz", 0755) != -1)
            {
                fprintf(stdout, "Initialized empty changiz repository");
                mkdir(".changiz/tags", 0755);
                mkdir(".changiz/stage", 0755);
                mkdir(".changiz/config", 0755);
                mkdir(".changiz/branches", 0755);
                mkdir(".changiz/data_saver", 0755);
                mkdir(".changiz/branches/masterbranch", 0755);
                FILE *current = fopen(".changiz/current_location", "w");
                fprintf(current, "masterbranch");
                fclose(current);
                FILE *commit_id = fopen(".changiz/current_commit_id", "w");
                fprintf(commit_id, "1");
                fclose(commit_id);
                FILE *fp = fopen(".changiz/save_staging_names", "w");
                fclose(fp);
                FILE *ID = fopen(".changiz/id_number", "w");
                fprintf(ID, "1");
                fclose(ID);
                FILE *log = fopen(".changiz/log_file", "w");
                fclose(log);
                FILE *author = fopen(".changiz/author_list", "w");
                fclose(author);
                FILE *branch = fopen(".changiz/branch_list", "w");
                fclose(branch);
                FILE *status = fopen(".changiz/status_file", "w");
                fclose(status);
                FILE *tag = fopen(".changiz/tag_list", "w");
                fclose(tag);
                FILE *list = fopen(".changiz/hook_list", "w");
                fclose(list);

                char command[MAX_COMMAND_LENGTH] = "";
                sprintf(command, "cp -r /Users/parmis/Desktop/project/FOP/config_global/config_user_name .changiz/config");
                system(command);

                char command_email[MAX_COMMAND_LENGTH] = "";
                sprintf(command_email, "cp -r /Users/parmis/Desktop/project/FOP/config_global/config_user_email .changiz/config");
                system(command_email);
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "changiz repository has already initialized");
        }
    }
    return 0;
}

int configs(int argc, char *const argv[])
{
    if (strcmp(argv[2], "-global") == 0)
    {
        if (strcmp(argv[3], "user.name") == 0)
        {
            FILE *file_name = fopen("/Users/parmis/Desktop/project/FOP/config_global/config_user_name", "w");
            fprintf(file_name, "%s", argv[4]);
            fprintf(stdout, "global user.name config succsessfully");
            fclose(file_name);
            return 1;
        }
        else if (strcmp(argv[3], "user.email") == 0)
        {
            FILE *file_email = fopen("/Users/parmis/Desktop/project/FOP/config_global/config_user_email", "w");
            fprintf(file_email, "%s", argv[4]);
            fprintf(stdout, "global user.email config succsessfully");
            fclose(file_email);
            return 1;
        }
    }
    else if (strcmp(argv[2], "user.name") == 0)
    {
        FILE *file_rn_name = fopen(".changiz/config/config_user_name", "w");
        fprintf(file_rn_name, "%s", argv[3]);
        fprintf(stdout, "user.name config succsessfully");
        fclose(file_rn_name);
        return 1;
    }
    else if (strcmp(argv[2], "user.email") == 0)
    {
        FILE *file_rn_email = fopen(".changiz/config/config_user_email", "w");
        fprintf(file_rn_email, "%s", argv[3]);
        fprintf(stdout, "user.email config succsessfully");
        fclose(file_rn_email);
        return 1;
    }
    return 0;
}

struct dirent *search_in_directory(char *search_file, char *address)
{
    struct dirent *entry;
    DIR *inside_dir = opendir(address);
    while ((entry = readdir(inside_dir)) != NULL)
    {
        if (strcmp(entry->d_name, search_file) == 0)
        {
            return entry;
        }
    }
    return NULL;
}
struct dirent *HEAD_finder(char *address, int n)
{
    struct dirent *entry;
    DIR *inside_address = opendir(address);
    int max_id = 0;
    int prev_max = 100;
    for (int i = 0; i < n; i++)
    {
        max_id = 0;
        while ((entry = readdir(inside_address)) != NULL)
        {
            int temp = atoi(entry->d_name);
            if (temp == 0)
            {
                continue;
            }
            if ((temp > max_id) && (temp < prev_max))
            {
                max_id = temp;
            }
        }
        prev_max = max_id;
        rewinddir(inside_address);
    }
    char str_id[MAX_NAME_LENGTH] = "";
    sprintf(str_id, "%d", max_id);
    rewinddir(inside_address);
    while ((entry = readdir(inside_address)) != NULL)
    {
        if (strcmp(entry->d_name, str_id) == 0)
        {
            return entry;
        }
    }
    return NULL;
}

int check_for_add(char *filename, char *address)
{
    struct dirent *check;
    DIR *dir_stage = opendir(address);
    while ((check = readdir(dir_stage)) != NULL)
    {
        if (strcmp(check->d_name, filename) == 0)
        {
            char file_location[MAX_FILENAME_LENGTH] = "";
            strcpy(file_location, address);
            strcat(file_location, "/");
            strcat(file_location, check->d_name);

            char staged_file_location[MAX_FILENAME_LENGTH] = ".changiz/stage/";
            strcat(staged_file_location, address);
            strcat(staged_file_location, "/");
            strcat(staged_file_location, check->d_name);

            if (check->d_type == DT_REG)
            {
                char str1[10000];
                char str2[10000];

                FILE *new_file = fopen(file_location, "r");
                FILE *check_file = fopen(staged_file_location, "r");

                fscanf(new_file, "%[^\0]s", str1);
                fscanf(check_file, "%[^\0]s", str2);
                if (strcmp(str1, str2) != 0)
                {
                    return 0;
                }
                else if (strcmp(str1, str2) == 0)
                {
                    return 1;
                }
            }

            else if (check->d_type == DT_DIR)
            {
                DIR *inside_dir = opendir(file_location);
                struct dirent *check_inside;
                while ((check_inside = readdir(inside_dir)) != NULL)
                {
                    if ((strcmp(check_inside->d_name, ".") == 0) || (strcmp(check_inside->d_name, "..") == 0) || (strcmp(check_inside->d_name, ".DS_Store") == 0))
                    {
                        continue;
                    }
                    struct dirent *entry = search_in_directory(check_inside->d_name, staged_file_location);
                    if (entry == NULL)
                    {
                        return 0;
                    }
                    if (check_for_add(check_inside->d_name, file_location) == 0)
                    {
                        return 0;
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}
int add(int argc, char *const argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "please specify a file\n");
        return 1;
    }
    if (strcmp(argv[2], "-redo") == 0)
    {
        struct dirent *entry_stage;
        DIR *dir_stage = opendir(".changiz/stage");
        if (dir_stage == NULL)
        {
            fprintf(stderr, "Error opening current directory\n");
            return 1;
        }
        while ((entry_stage = readdir(dir_stage)) != NULL)
        {
            if ((strcmp(entry_stage->d_name, ".") == 0) || (strcmp(entry_stage->d_name, "..") == 0) || (strcmp(entry_stage->d_name, ".DS_Store") == 0))
            {
                continue;
            }
            char command_redo[MAX_COMMAND_LENGTH] = "";
            sprintf(command_redo, "rsync -r %s %s", entry_stage->d_name, ".changiz/stage");
            system(command_redo);
        }
        fprintf(stdout, "All files and directories added successfully\n");
        return 1;
    }
    if (strcmp(argv[2], "-n") == 0)
    {
        char save_name[1000][1000];
        int name_counter = 0;
        struct dirent *staged_entry;
        DIR *dir_first = opendir(".");
        while ((staged_entry = readdir(dir_first)) != NULL)
        {
            if ((strcmp(staged_entry->d_name, ".") == 0) || (strcmp(staged_entry->d_name, "..") == 0) ||
                (strcmp(staged_entry->d_name, ".DS_Store") == 0))
            {
                continue;
            }
            struct dirent *find_entry = search_in_directory(staged_entry->d_name, ".changiz/stage/");
            if (find_entry != NULL)
            {
                if (find_entry->d_type == DT_REG)
                {
                    if (check_for_add(find_entry->d_name, ".") == 0)
                    {
                        strcpy(save_name[name_counter], find_entry->d_name);
                        strcat(save_name[name_counter], " (unstage)");
                        fprintf(stdout, "%s \n", save_name[name_counter]);
                        name_counter++;
                    }
                    else if (check_for_add(find_entry->d_name, ".") == 1)
                    {
                        strcpy(save_name[name_counter], find_entry->d_name);
                        strcat(save_name[name_counter], " (stage)");
                        fprintf(stdout, "%s \n", save_name[name_counter]);
                        name_counter++;
                    }
                }
                if (find_entry->d_type == DT_DIR)
                {
                    if (check_for_add(staged_entry->d_name, ".") == 0)
                    {
                        strcpy(save_name[name_counter], staged_entry->d_name);
                        strcat(save_name[name_counter], " (unstage)");
                        fprintf(stdout, "%s \n", save_name[name_counter]);
                        name_counter++;
                    }
                    else if (check_for_add(staged_entry->d_name, ".") == 1)
                    {
                        strcpy(save_name[name_counter], staged_entry->d_name);
                        strcat(save_name[name_counter], " (stage)");
                        fprintf(stdout, "%s \n", save_name[name_counter]);
                        name_counter++;
                    }
                }
            }

            else
            {
                strcpy(save_name[name_counter], staged_entry->d_name);
                strcat(save_name[name_counter], " (unstage)");
                fprintf(stdout, "%s \n", save_name[name_counter]);
                name_counter++;
            }
        }
        return 1;
    }
    int f = 0;
    if (strcmp(argv[2], "-f") == 0)
    {
        f++;
    }
    for (int i = 2 + f; i < argc; i++)
    {
        int exist = 0;
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            fprintf(stderr, "Error opening current directory\n");
            return 1;
        }
        closedir(dir);
        char filename[MAX_FILENAME_LENGTH] = "";
        char dir_name[MAX_FILENAME_LENGTH] = "";
        char *ptr_slash = strrchr(argv[i], '/');
        if (ptr_slash != NULL)
        {
            strcpy(filename, ptr_slash + 1);
            strncpy(dir_name, argv[i], ptr_slash - argv[i]);
        }
        else
        {
            strcpy(filename, argv[i]);
            strcpy(dir_name, ".");
        }
        struct dirent *entry;
        DIR *dir_check = opendir(dir_name);
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, filename) == 0)
            {
                exist = 1;
                int is_staged = 0;
                struct dirent *stage_entry = search_in_directory(filename, ".changiz/stage");
                if (stage_entry != NULL)
                {
                    is_staged = check_for_add(argv[i], ".");
                }
                if (!is_staged)
                {
                    if (entry->d_type == DT_REG)
                    {
                        DIR *dir_stage = opendir(".changiz/stage");
                        char command[MAX_COMMAND_LENGTH] = "";
                        sprintf(command, "rsync -r %s %s/%s/", argv[i], ".changiz/stage", dir_name);
                        system(command);
                        FILE *fp = fopen(".changiz/save_staging_names", "a");
                        fprintf(fp, "%s|", argv[i]);
                        if (argc < 4)
                        {
                            fprintf(fp, "\n");
                        }
                        fclose(fp);
                        fprintf(stdout, "file %s added successfully\n", argv[i]);
                        closedir(dir);
                        break;
                    }
                    else if (entry->d_type == DT_DIR)
                    {
                        DIR *dir_stage = opendir(".changiz/stage");
                        char command[MAX_COMMAND_LENGTH] = "";
                        sprintf(command, "rsync -r %s %s/%s/", argv[i], ".changiz/stage", dir_name);
                        system(command);
                        FILE *fp = fopen(".changiz/save_staging_names", "a");
                        fprintf(fp, "%s|", argv[i]);
                        if (argc < 4)
                        {
                            fprintf(fp, "\n");
                        }
                        fclose(fp);
                        fprintf(stdout, "directory %s added successfully\n", argv[i]);
                        closedir(dir);
                        break;
                    }
                }
                else
                {
                    fprintf(stderr, "file is already staged\n");
                    break;
                }
            }
        }
        if (!exist)
        {
            fprintf(stdout, "file or directory doesn't exist\n");
        }
    }
    return 0;
}

int reset(int argc, char *const argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "please specify a file\n");
        return 1;
    }
    if (strcmp(argv[2], "-undo") == 0)
    {
        int line_counter = 0;
        int count_name = 0;
        char save_file[1000] = "";
        char save_line[1000][1000];
        char save_name[1000][1000];
        FILE *fp = fopen(".changiz/save_staging_names", "r");
        fscanf(fp, "%[^\0]s", save_file);
        fclose(fp);
        if (strlen(save_file) == 0)
        {
            fprintf(stdout, "Nothing exists to reset\n");
            return 0;
        }
        char *ptr_line = strtok(save_file, "\n");
        while (ptr_line != NULL)
        {
            strcpy(save_line[line_counter], ptr_line);
            line_counter++;
            ptr_line = strtok(NULL, "\n");
        }
        char *ptr_name = strtok(save_line[line_counter - 1], "|");
        while (ptr_name != NULL)
        {
            struct dirent *exist = search_in_directory(ptr_name, ".changiz/stage/");
            if (exist == NULL)
            {
                fprintf(stderr, "No such a file or directory exist");
                FILE *edit = fopen(".changiz/save_staging_names", "w");
                for (int i = 0; i < line_counter - 1; i++)
                {
                    fprintf(edit, "%s\n", save_line[i]);
                }
                fclose(edit);
                return 0;
            }
            char command[MAX_COMMAND_LENGTH] = "";
            sprintf(command, "rm -r .changiz/stage/%s", ptr_name);
            system(command);
            count_name++;
            ptr_name = strtok(NULL, "|");
        }
        FILE *fp2 = fopen(".changiz/save_staging_names", "w");
        for (int i = 0; i < line_counter - 1; i++)
        {
            fprintf(fp, "%s\n", save_line[i]);
        }
        fclose(fp2);
        fprintf(stdout, "files or directories undo successfully\n");
        return 0;
    }
    int f = 0;
    if (strcmp(argv[2], "-f") == 0)
    {
        f++;
    }
    for (int i = 2 + f; i < argc; i++)
    {
        int exist = 0;
        struct dirent *entry;
        DIR *dir = opendir(".changiz/stage");
        if (dir == NULL)
        {
            fprintf(stderr, "Nothing exist to reset\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, argv[i]) == 0)
            {
                exist = 1;
                if (entry->d_type == DT_REG)
                {
                    char command[MAX_COMMAND_LENGTH] = "";
                    sprintf(command, "rm -r .changiz/stage/%s", argv[i]);
                    system(command);
                    fprintf(stdout, "file %s reset successfully\n", argv[i]);
                    closedir(dir);
                    break;
                }
                else if (entry->d_type == DT_DIR)
                {
                    char command[MAX_COMMAND_LENGTH] = "";
                    sprintf(command, "rm -r .changiz/stage/%s", argv[i]);
                    system(command);
                    fprintf(stdout, "directory %s reset successfully\n", argv[i]);
                    closedir(dir);
                    break;
                }
            }
        }
        if (!exist)
        {
            fprintf(stdout, "file or directory doesn't exist to reset\n");
        }
    }
    return 0;
}

int commit(int argc, char *const argv[])
{
    if (argc != 4)
    {
        fprintf(stdout, "please enter a valid command");
        return 1;
    }
    if ((strlen(argv[3]) > 72) || (strlen(argv[3]) == 0))
    {
        fprintf(stdout, "please enter a valid message");
        return 1;
    }
    if (strcmp(argv[2], "-s") == 0)
    {
        int flag_not_exist = 0;
        find_shortcut(argv[3], &flag_not_exist);
        if (flag_not_exist == 1)
        {
            return 0;
        }
    }

    FILE *count = fopen(".changiz/save_staging_names", "r");
    if (fgetc(count) == EOF)
    {
        fprintf(stderr, "Nothing exist to commit");
        return 1;
    }
    char stage_string[MAX_NAME_LENGTH] = "";
    fscanf(count, "%[^\0]s", stage_string);
    int counter = 0;
    char *token = strtok(stage_string, "|");
    while (token != NULL)
    {
        token = strtok(NULL, "|");
        counter++;
    }
    counter--;
    fclose(count);
    FILE *delete = fopen(".changiz/save_staging_names", "w");
    fclose(delete);

    char str_id[100] = "";
    FILE *ID = fopen(".changiz/id_number", "r");
    fscanf(ID, "%[^\0]s", str_id);
    fclose(ID);

    char temp_current_branch[MAX_NAME_LENGTH] = "";
    char branch[MAX_NAME_LENGTH] = "";
    FILE *current = fopen(".changiz/current_location", "r");
    fscanf(current, "%[^\0]s", temp_current_branch);
    strcpy(branch, temp_current_branch);
    char current_location[MAX_COMMAND_LENGTH] = ".changiz/branches/";
    strcat(current_location, branch);

    char temp_current_id[MAX_NAME_LENGTH] = "";
    FILE *commit_id = fopen(".changiz/current_commit_id", "r");
    fscanf(commit_id, "%[^\0]s", temp_current_id);
    fclose(commit_id);
    strcat(current_location, "/");
    strcat(current_location, temp_current_id);

    char dest_location[MAX_NAME_LENGTH] = "";
    char *ptr_slash = strrchr(current_location, '/');
    if (ptr_slash != NULL)
    {
        strncpy(dest_location, current_location, ptr_slash - current_location);
    }
    strcat(dest_location, "/");
    strcat(dest_location, str_id);

    mkdir(dest_location, 0755);
    int id = atoi(str_id);
    if (id > 1)
    {
        char command_copy[MAX_COMMAND_LENGTH] = "";
        sprintf(command_copy, "cp -r %s/* %s", current_location, dest_location);
        system(command_copy);
    }
    id++;

    FILE *ID2 = fopen(".changiz/id_number", "w");
    fprintf(ID2, "%d", id);
    fclose(ID2);

    char command[MAX_COMMAND_LENGTH] = "";
    sprintf(command, "mv .changiz/stage/* %s", dest_location);
    system(command);

    char author_name[MAX_NAME_LENGTH] = "";
    FILE *name = fopen(".changiz/config/config_user_name", "r");
    fscanf(name, "%[^\0]s", author_name);
    fclose(name);

    char author_email[MAX_NAME_LENGTH] = "";
    FILE *email = fopen(".changiz/config/config_user_email", "r");
    fscanf(email, "%[^\0]s", author_email);
    fclose(email);

    time_t cur = time(NULL);
    char current_time[1000] = "";
    strcpy(current_time, ctime(&cur));

    FILE *log = fopen(".changiz/log_file", "r");
    char last_log[100000] = "";
    fscanf(log, "%[^\0]s", last_log);
    fclose(log);

    FILE *creat_log = fopen(".changiz/log_file", "w");
    fprintf(creat_log, "Commit id: %s\nComment: %s\nDate: %sAuthor: %s %s\nCount of commits: %d\nOn branch: %s\n\n", str_id, argv[3], current_time, author_name, author_email, counter, branch);
    fprintf(creat_log, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\n");
    fclose(creat_log);

    char creat_data_file[MAX_FILENAME_LENGTH] = ".changiz/data_saver/";
    strcat(creat_data_file, str_id);
    FILE *write_data = fopen(creat_data_file, "w");
    fprintf(write_data, "Commit id: %s\nComment: %s\nDate: %sAuthor: %s %s\nCount of commits: %d\nOn branch: %s\n", str_id, argv[3], current_time, author_name, author_email, counter, branch);
    fclose(write_data);

    FILE *complete_log = fopen(".changiz/log_file", "a");
    fprintf(complete_log, "%s", last_log);
    fclose(complete_log);

    FILE *author = fopen(".changiz/author_list", "a");
    fprintf(author, "(%s) %s\n", str_id, author_name);
    fclose(author);

    FILE *save_branch = fopen(".changiz/branch_list", "a");
    fprintf(save_branch, "(%s) %s\n", str_id, branch);
    fclose(save_branch);

    FILE *add_commit_id = fopen(".changiz/current_commit_id", "w");
    fprintf(add_commit_id, "%s", str_id);
    fclose(add_commit_id);

    fprintf(stdout, "commited successfully\n\n");
    fprintf(stdout, "›Commit id: %s\n›Comment: %s\n›Date: %s", str_id, argv[3], ctime(&cur));
    return 1;
}

int set(int argc, char *const argv[])
{
    if (argc < 3 || argv[5] == NULL)
    {
        fprintf(stderr, "please enter a valid command\n");
        return 0;
    }
    FILE *shortcut = fopen(".changiz/saving_shortcuts", "a");
    fprintf(shortcut, "%s\n%s\n", argv[5], argv[3]);
    fclose(shortcut);
    fprintf(stdout, "Shortcut added successfully");
    return 1;
}

void find_shortcut(char *shortcut_name, int *flag_not_exist)
{
    FILE *shortcut = fopen(".changiz/saving_shortcuts", "r");
    char file_str[MAX_FILENAME_LENGTH] = "";
    char shortcut_line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int line_counter = 0;

    fscanf(shortcut, "%[^\0]s", file_str);
    fclose(shortcut);
    char *line = strtok(file_str, "\n");
    while (line != NULL)
    {
        strcpy(shortcut_line[line_counter], line);
        line_counter++;
        line = strtok(NULL, "\n");
    }
    int flag = 0;
    for (int i = 0; i < line_counter; i++)
    {
        if (strcmp(shortcut_line[i], shortcut_name) == 0)
        {
            strcpy(shortcut_name, shortcut_line[i + 1]);
            flag = 1;
            break;
        }
    }
    if (!flag)
    {
        fprintf(stderr, "this shortcut doesn't exist,please enter a valid command\n");
        *flag_not_exist = 1;
        return;
    }
    return;
}

int replace(int argc, char *const argv[])
{
    if (argc < 3 || argv[5] == NULL)
    {
        fprintf(stderr, "please enter a valid command\n");
        return 0;
    }
    FILE *shortcut = fopen(".changiz/saving_shortcuts", "r");
    char file_str[MAX_FILENAME_LENGTH] = "";
    char shortcut_line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int line_counter = 0;

    fscanf(shortcut, "%[^\0]s", file_str);
    fclose(shortcut);
    char *line = strtok(file_str, "\n");
    while (line != NULL)
    {
        strcpy(shortcut_line[line_counter], line);
        line_counter++;
        line = strtok(NULL, "\n");
    }
    int flag = 0;
    for (int i = 0; i < line_counter; i++)
    {
        if (strcmp(shortcut_line[i], argv[5]) == 0)
        {
            strcpy(shortcut_line[i + 1], argv[3]);
            flag = 1;
            break;
        }
    }
    if (!flag)
    {
        fprintf(stderr, "this shortcut doesn't exist,please enter a valid command");
        return 0;
    }
    FILE *edited = fopen(".changiz/saving_shortcuts", "w");
    for (int j = 0; j < line_counter; j++)
    {
        fprintf(edited, "%s\n", shortcut_line[j]);
    }
    fclose(edited);
    fprintf(stdout, "Shortcut changed successfully");
    return 1;
}

int remove_func(int argc, char *const argv[])
{
    if (argv[3] == NULL)
    {
        fprintf(stderr, "please enter a valid command\n");
        return 0;
    }
    FILE *shortcut = fopen(".changiz/saving_shortcuts", "r");
    char file_str[MAX_FILENAME_LENGTH] = "";
    char shortcut_line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int line_counter = 0;

    fscanf(shortcut, "%[^\0]s", file_str);
    fclose(shortcut);
    char *line = strtok(file_str, "\n");
    while (line != NULL)
    {
        strcpy(shortcut_line[line_counter], line);
        line_counter++;
        line = strtok(NULL, "\n");
    }
    int flag = 0;
    for (int i = 0; i < line_counter; i++)
    {
        if (strcmp(shortcut_line[i], argv[3]) == 0)
        {
            flag = 1;
            if (i == line_counter - 2)
            {
                break;
            }
            for (int j = i; j < line_counter; j++)
            {
                strcpy(shortcut_line[j], shortcut_line[j + 2]);
            }
            break;
        }
    }
    if (!flag)
    {
        fprintf(stderr, "this shortcut doesn't exist,please enter a valid command\n");
        return 0;
    }
    strcpy(shortcut_line[line_counter - 1], "\0");
    strcpy(shortcut_line[line_counter - 2], "\0");
    FILE *edited = fopen(".changiz/saving_shortcuts", "w");
    for (int z = 0; z < line_counter - 2; z++)
    {
        fprintf(edited, "%s\n", shortcut_line[z]);
    }
    fclose(edited);
    fprintf(stdout, "Shortcut removed successfully");
    return 1;
}

int branch(int argc, char *const argv[])
{
    if (argc == 3)
    {
        struct dirent *entry = search_in_directory(argv[2], ".changiz/branches");
        if (entry == NULL)
        {
            char dest_location[MAX_COMMAND_LENGTH] = ".changiz/branches/";
            strcat(dest_location, argv[2]);
            mkdir(dest_location, 0755);

            char last_commit[1000] = "";
            FILE *ID = fopen(".changiz/id_number", "r");
            fscanf(ID, "%[^\0]s", last_commit);
            strcat(dest_location, "/");
            strcat(dest_location, last_commit);
            mkdir(dest_location, 0755);
            fclose(ID);

            char temp_current_branch[MAX_NAME_LENGTH] = "";
            FILE *current = fopen(".changiz/current_location", "r");
            fscanf(current, "%[^\0]s", temp_current_branch);
            fclose(current);
            char current_location[MAX_COMMAND_LENGTH] = ".changiz/branches/";
            strcat(current_location, temp_current_branch);

            char temp_current_id[MAX_NAME_LENGTH] = "";
            FILE *commit_id = fopen(".changiz/current_commit_id", "r");
            fscanf(commit_id, "%[^\0]s", temp_current_id);
            fclose(commit_id);

            strcat(current_location, "/");
            strcat(current_location, temp_current_id);

            char command[MAX_COMMAND_LENGTH] = "";
            sprintf(command, "cp -r %s/* %s", current_location, dest_location);
            system(command);
            fprintf(stdout, "Branch '%s' set up", argv[2]);

            FILE *ID_plus = fopen(".changiz/id_number", "w");
            int last_id = atoi(last_commit);
            last_id++;
            fprintf(ID_plus, "%d", last_id);

            FILE *add_branch = fopen(".changiz/branch_list", "a");
            fprintf(add_branch, "(%s) %s\n", last_commit, argv[2]);
            fclose(add_branch);

            return 1;
        }
        else
        {
            fprintf(stderr, "A branch with this name already exist\n");
            return 0;
        }
    }
    if (argc == 2)
    {
        struct dirent *entry;
        DIR *branch_check = opendir(".changiz/branches");
        while ((entry = readdir(branch_check)) != NULL)
        {
            fprintf(stdout, "->%s\n", entry->d_name);
            return 1;
        }
        fprintf(stdout, "No branch exist\n");
        return 0;
    }
    fprintf(stderr, "please enter a valid command");
    return 0;
}

int log_func(int argc, char *const argv[])
{
    if (argc == 2)
    {
        FILE *log_check = fopen(".changiz/log_file", "r");
        if (log_check == NULL)
        {
            fprintf(stderr, "Nothing match with your request,please try agian");
            return 0;
        }
        fclose(log_check);
        char command[MAX_COMMAND_LENGTH] = "";
        sprintf(command, "cat .changiz/log_file");
        system(command);
        return 1;
    }
    int till = 0;
    char check_author[MAX_NAME_LENGTH] = "";
    char check_branch[MAX_NAME_LENGTH] = "";
    int last_id;
    FILE *ID = fopen(".changiz/id_number", "r");
    fscanf(ID, "%d", &last_id);
    fclose(ID);
    last_id--;

    if (argc > 2)
    {
        if (strcmp(argv[2], "-n") == 0)
        {
            if (argc != 4)
            {
                fprintf(stderr, "please enter a valid command");
                return 1;
            }
            till = last_id - atoi(argv[3]);
        }
        if (strcmp(argv[2], "-branch") == 0)
        {
            if (argc != 4)
            {
                fprintf(stderr, "please enter a valid command");
                return 1;
            }
            struct dirent *search = search_in_directory(argv[3], ".changiz/branches");
            if (search == NULL)
            {
                fprintf(stderr, "This branch doesn't exist");
                return 1;
            }
            FILE *find_branch = fopen(".changiz/branch_list", "r");
            fscanf(find_branch, "%[^\0]s", check_branch);
            fclose(find_branch);
        }
        if (strcmp(argv[2], "-author") == 0)
        {
            if (argc != 4)
            {
                fprintf(stderr, "please enter a valid command");
                return 1;
            }
            FILE *find_author = fopen(".changiz/author_list", "r");
            fscanf(find_author, "%[^\0]s", check_author);
            fclose(find_author);
        }
    }
    for (int i = last_id; i > till; i--)
    {
        if (argc > 2 && strcmp(argv[2], "-branch") == 0)
        {
            char look_for[MAX_NAME_LENGTH] = "";
            sprintf(look_for, "(%d) %s", i, argv[3]);
            char *find = strstr(check_branch, look_for);
            if (find == NULL)
            {
                continue;
            }
        }
        if (argc > 2 && strcmp(argv[2], "-author") == 0)
        {
            char look_for[MAX_NAME_LENGTH] = "";
            sprintf(look_for, "(%d) %s", i, argv[3]);
            char *find = strstr(check_author, look_for);
            if (find == NULL)
            {
                continue;
            }
        }
        char str_id[100] = "";
        sprintf(str_id, "%d", i);
        char src_location[MAX_NAME_LENGTH] = "";
        strcpy(src_location, ".changiz/data_saver/");
        strcat(src_location, str_id);
        if (argc > 2 && strcmp(argv[2], "-search") == 0)
        {
            int flag_exist = 0;
            for (int j = 3; j < argc; j++)
            {
                FILE *search_file = fopen(src_location, "r");
                char search_temp[MAX_MESSAGE_LENGTH] = "";
                fscanf(search_file, "%[^\0]s", search_temp);
                fclose(search_file);
                char *find_word = strstr(search_temp, argv[i]);
                if (find_word != NULL)
                {
                    flag_exist = 1;
                    break;
                }
            }
            if (!flag_exist)
            {
                continue;
            }
        }
        if (argc > 2 && strcmp(argv[2], "-since") == 0)
        {
            fprintf(stderr, "here with commit id %d\n", i);
            struct stat file_stat;
            time_t creation_time;
            if (stat(src_location, &file_stat) == 0)
            {
                creation_time = file_stat.st_mtime;
            }
            struct tm time_temp;
            if (strptime(argv[3], "%Y-%m-%d %H:%M:%S", &time_temp) == NULL)
            {
                return -1;
            }
            time_t time_compare = mktime(&time_temp);
            double difference = difftime(creation_time, time_compare);
            if (difference < 0)
            {
                continue;
            }
        }
        if (argc > 2 && strcmp(argv[2], "-before") == 0)
        {
            FILE *search_date = fopen(src_location, "r");
            struct stat file_stat;
            time_t creation_time;
            if (stat(src_location, &file_stat) == 0)
            {
                creation_time = file_stat.st_mtime;
            }
            struct tm time_temp;
            if (strptime(argv[3], "%Y-%m-%d %H:%M:%S", &time_temp) == NULL)
            {
                return -1;
            }
            time_t time_compare = mktime(&time_temp);
            double difference = difftime(creation_time, time_compare);
            if (difference > 0)
            {
                continue;
            }
        }
        char temp[10000] = "";
        FILE *data = fopen(src_location, "r");
        fscanf(data, "%[^\r]s", temp);
        fclose(data);
        FILE *log = fopen(".changiz/print_log", "a");
        fprintf(log, "%s\n\n", temp);
        fprintf(log, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\n");
        fclose(log);
    }
    FILE *log_checker = fopen(".changiz/print_log", "r");
    if (log_checker == NULL)
    {
        fprintf(stderr, "Nothing match with your request,please try agian");
        return 0;
    }
    fclose(log_checker);
    char command_cat[MAX_COMMAND_LENGTH] = "";
    sprintf(command_cat, "cat .changiz/print_log");
    system(command_cat);
    FILE *delete = fopen(".changiz/print_log", "w");
    fclose(delete);
    return 0;
}

int checkout(int argc, char *const argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "please enter a valid command");
        return 1;
    }

    FILE *check_stage = fopen(".changiz/save_staging_names", "r");
    if (fgetc(check_stage) != EOF)
    {
        fprintf(stderr, "you need to commit changes first!");
        return 0;
    }
    fclose(check_stage);

    char string_id[10000] = "";
    strcpy(string_id, argv[2]);
    if (atoi(argv[2]) == 0)
    {
        if (strcmp(argv[2], "HEAD") != 0)
        {
            char branch_address[MAX_FILENAME_LENGTH] = ".changiz/branches/";
            strcat(branch_address, argv[2]);
            strcpy(string_id, "/");
            strcpy(string_id, HEAD_finder(branch_address, 1)->d_name);
        }
        else
        {
            char temp_current_branch[MAX_NAME_LENGTH] = "";
            FILE *current = fopen(".changiz/current_location", "r");
            fscanf(current, "%[^\0]s", temp_current_branch);
            fclose(current);
            char branch_address[MAX_FILENAME_LENGTH] = ".changiz/branches/";
            strcat(branch_address, temp_current_branch);
            strcpy(string_id, HEAD_finder(branch_address, atoi(argv[3]))->d_name);
        }
    }
    char check_branch[MAX_FILENAME_LENGTH] = "";
    FILE *find_branch = fopen(".changiz/branch_list", "r");
    fscanf(find_branch, "%[^\0]s", check_branch);
    fclose(find_branch);

    char *find = strstr(check_branch, string_id);
    if (find == NULL)
    {
        fprintf(stderr, "This commit id doesn't exist");
        return 0;
    }
    else
    {
        char branch[MAX_NAME_LENGTH] = "";
        sscanf(find + 3, "%s", branch);

        char temp_current_branch[MAX_NAME_LENGTH] = "";
        FILE *current = fopen(".changiz/current_location", "r");
        fscanf(current, "%[^\0]s", temp_current_branch);
        fclose(current);
        char current_location[MAX_COMMAND_LENGTH] = ".changiz/branches/";
        strcat(current_location, temp_current_branch);

        char temp_current_id[MAX_NAME_LENGTH] = "";
        FILE *commit_id = fopen(".changiz/current_commit_id", "r");
        fscanf(commit_id, "%[^\0]s", temp_current_id);
        fclose(commit_id);
        strcat(current_location, "/");
        strcat(current_location, temp_current_id);

        struct dirent *local_files;
        DIR *local = opendir(".");
        while ((local_files = readdir(local)) != NULL)
        {
            if (local_files->d_name[0] == '.')
            {
                continue;
            }
            struct dirent *track = search_in_directory(local_files->d_name, current_location);
            if (track == NULL)
            {
                fprintf(stderr, "You can't checkout because %s is not commit yet", local_files->d_name);
                return 0;
            }
        }
        closedir(local);

        struct dirent *delete_files;
        DIR *local_clean = opendir(".");
        while ((delete_files = readdir(local)) != NULL)
        {
            if (delete_files->d_name[0] == '.')
            {
                continue;
            }
            char command_rm[MAX_COMMAND_LENGTH] = "";
            sprintf(command_rm, "rm -r %s", delete_files->d_name);
            system(command_rm);
        }
        closedir(local_clean);

        char new_current[MAX_FILENAME_LENGTH] = ".changiz/branches/";
        strcat(new_current, branch);
        strcat(new_current, "/");
        strcat(new_current, string_id);

        char command[MAX_COMMAND_LENGTH] = "";
        sprintf(command, "cp -r %s/* %s", new_current, ".");
        system(command);

        FILE *add_current = fopen(".changiz/current_location", "w");
        fprintf(add_current, "%s", branch);
        fclose(add_current);

        FILE *add_commit_id = fopen(".changiz/current_commit_id", "w");
        fprintf(add_commit_id, "%s", string_id);
        fclose(add_commit_id);

        fprintf(stdout, "Checkout to commit %s done successfully", string_id);
        return 1;
    }
}

int status(int argc, char *const argv[])
{
    char last_id[MAX_NAME_LENGTH] = "";
    FILE *ID = fopen(".changiz/id_number", "r");
    fscanf(ID, "%[^\0]s", last_id);
    int temp_id = atoi(last_id);
    temp_id--;
    char temp_last_id[MAX_NAME_LENGTH] = "";
    sprintf(temp_last_id, "%d", temp_id);
    fclose(ID);

    char check_branch[MAX_FILENAME_LENGTH] = "";
    FILE *find_branch = fopen(".changiz/branch_list", "r");
    fscanf(find_branch, "%[^\0]s", check_branch);
    fclose(find_branch);

    char *find = strstr(check_branch, temp_last_id);
    char branch[MAX_NAME_LENGTH] = "";
    sscanf(find + 3, "%s", branch);

    char compare_commit[MAX_NAME_LENGTH] = "";
    sprintf(compare_commit, ".changiz/branches/%s/%s", branch, temp_last_id);

    struct dirent *commit_entry;
    DIR *inside_last_commit = opendir(compare_commit);
    while ((commit_entry = readdir(inside_last_commit)) != NULL)
    {
        if (commit_entry->d_type == DT_DIR || (strcmp(commit_entry->d_name, ".") == 0) || (strcmp(commit_entry->d_name, "..") == 0) || (strcmp(commit_entry->d_name, ".DS_Store") == 0))
        {
            continue;
        }
        struct dirent *entry = search_in_directory(commit_entry->d_name, ".");
        if (entry == NULL)
        {
            char situation = '+';
            struct dirent *entry_stage = search_in_directory(commit_entry->d_name, ".changiz/stage");
            if (entry_stage == NULL)
            {
                situation = '-';
            }

            FILE *complete_status = fopen(".changiz/status_file", "a");
            if (situation == '+')
            {
                fprintf(complete_status, "›%s -> +D\n", commit_entry->d_name);
                fclose(complete_status);
                continue;
            }
            if (situation == '-')
            {
                fprintf(complete_status, "›%s -> -D\n", commit_entry->d_name);
                fclose(complete_status);
                continue;
            }
        }
        else
        {
            char situation = '+';
            struct dirent *entry_stage = search_in_directory(entry->d_name, ".changiz/stage");

            if (entry_stage == NULL)
            {
                situation = '-';
            }

            FILE *complete_status = fopen(".changiz/status_file", "a");
            if (situation == '+')
            {
                char compare_file[MAX_NAME_LENGTH] = "";
                sprintf(compare_file, "%s/%s", compare_commit, commit_entry->d_name);
                char str1[10000];
                char str2[10000];

                FILE *local_file = fopen(entry->d_name, "r");
                FILE *check_file = fopen(compare_file, "r");

                fscanf(local_file, "%[^\0]s", str1);
                fscanf(check_file, "%[^\0]s", str2);
                fclose(local_file);
                fclose(check_file);
                if (strcmp(str1, str2) != 0)
                {
                    fprintf(complete_status, "›%s -> +M\n", commit_entry->d_name);
                    fclose(complete_status);
                    continue;
                }
            }
            if (situation == '-')
            {
                char compare_file[MAX_NAME_LENGTH] = "";
                sprintf(compare_file, "%s/%s", compare_commit, commit_entry->d_name);

                char str1[10000];
                char str2[10000];

                FILE *local_file = fopen(entry->d_name, "r");
                FILE *check_file = fopen(compare_file, "r");

                fscanf(local_file, "%[^\0]s", str1);
                fscanf(check_file, "%[^\0]s", str2);
                fclose(local_file);
                fclose(check_file);
                if (strcmp(str1, str2) != 0)
                {
                    fprintf(complete_status, "›%s -> -M\n", commit_entry->d_name);
                    fclose(complete_status);
                    continue;
                }
            }
        }
    }
    closedir(inside_last_commit);
    struct dirent *entry_local;
    DIR *dir_local = opendir(".");
    while ((entry_local = readdir(dir_local)) != NULL)
    {
        if (entry_local->d_type == DT_DIR || (strcmp(entry_local->d_name, ".") == 0) || (strcmp(entry_local->d_name, "..") == 0) || (strcmp(entry_local->d_name, ".DS_Store") == 0))
        {
            continue;
        }
        struct dirent *entry = search_in_directory(entry_local->d_name, compare_commit);
        if (entry == NULL)
        {
            char situation = '+';
            struct dirent *entry_stage = search_in_directory(entry_local->d_name, ".changiz/stage");
            if (entry_stage == NULL)
            {
                situation = '-';
            }

            FILE *complete_status = fopen(".changiz/status_file", "a");
            if (situation == '+')
            {
                fprintf(complete_status, "›%s -> +A\n", entry_local->d_name);
                fclose(complete_status);
                continue;
            }
            if (situation == '-')
            {
                fprintf(complete_status, "›%s -> -A\n", entry_local->d_name);
                fclose(complete_status);
                continue;
            }
        }
    }
    char command_cat[MAX_COMMAND_LENGTH] = "";
    sprintf(command_cat, "cat .changiz/status_file");
    system(command_cat);
    FILE *delete_status = fopen(".changiz/status_file", "w");
    fclose(delete_status);
    return 1;
}

int revert(int argc, char *const argv[])
{
    int which_argv = 2;
    if (strcmp(argv[2], "-m") == 0)
    {
        which_argv = 4;
    }
    if (strcmp(argv[2], "-n") == 0)
    {
        which_argv = 3;
    }

    char find_current_branch[MAX_NAME_LENGTH] = "";
    FILE *current_branch = fopen(".changiz/current_location", "r");
    fscanf(current_branch, "%[^\0]s", find_current_branch);
    fclose(current_branch);
    char find_current_location[MAX_COMMAND_LENGTH] = ".changiz/branches/";
    strcat(find_current_location, find_current_branch);

    if (strcmp(argv[2], "HEAD") == 0)
    {
        which_argv = atoi(HEAD_finder(find_current_location, atoi(argv[3]))->d_name);
    }
    else if (strcmp(argv[4], "HEAD") == 0)
    {
        which_argv = atoi(HEAD_finder(find_current_location, atoi(argv[5]))->d_name);
    }

    char check_branch[MAX_FILENAME_LENGTH] = "";
    FILE *find_branch = fopen(".changiz/branch_list", "r");
    fscanf(find_branch, "%[^\0]s", check_branch);
    fclose(find_branch);

    char *find = strstr(check_branch, argv[which_argv]);
    if (find == NULL)
    {
        fprintf(stderr, "This commit id doesn't exist");
        return 0;
    }
    else
    {
        char branch[MAX_NAME_LENGTH] = "";
        sscanf(find + 3, "%s", branch);

        char temp_current_branch[MAX_NAME_LENGTH] = "";
        FILE *current = fopen(".changiz/current_location", "r");
        fscanf(current, "%[^\0]s", temp_current_branch);
        fclose(current);
        char current_location[MAX_COMMAND_LENGTH] = ".changiz/branches/";
        strcat(current_location, temp_current_branch);

        char temp_current_id[MAX_NAME_LENGTH] = "";
        FILE *commit_id = fopen(".changiz/current_commit_id", "r");
        fscanf(commit_id, "%[^\0]s", temp_current_id);
        fclose(commit_id);
        strcat(current_location, "/");
        strcat(current_location, temp_current_id);

        struct dirent *local_files;
        DIR *local = opendir(".");
        while ((local_files = readdir(local)) != NULL)
        {
            if (local_files->d_name[0] == '.')
            {
                continue;
            }
            struct dirent *track = search_in_directory(local_files->d_name, current_location);
            if (track == NULL)
            {
                fprintf(stderr, "You can't revert because %s is not commit yet", local_files->d_name);
                return 0;
            }
        }
        closedir(local);

        struct dirent *delete_files;
        DIR *local_clean = opendir(".");
        while ((delete_files = readdir(local)) != NULL)
        {
            if (delete_files->d_name[0] == '.')
            {
                continue;
            }
            char command_rm[MAX_COMMAND_LENGTH] = "";
            sprintf(command_rm, "rm -r %s", delete_files->d_name);
            system(command_rm);
        }
        closedir(local_clean);

        char new_current[MAX_FILENAME_LENGTH] = ".changiz/branches/";
        strcat(new_current, branch);
        strcat(new_current, "/");
        strcat(new_current, argv[which_argv]);

        char command_copy_to_local[MAX_COMMAND_LENGTH] = "";
        sprintf(command_copy_to_local, "cp -r %s/* %s", new_current, ".");
        system(command_copy_to_local);

        if (strcmp(argv[2], "-n") == 0)
        {
            fprintf(stdout, "revert to commit %s done successfully", argv[which_argv]);
            return 1;
        }

        char new_commit[MAX_FILENAME_LENGTH] = ".changiz/branches/";
        strcat(new_commit, branch);
        char last_commit[1000] = "";
        FILE *ID = fopen(".changiz/id_number", "r");
        fscanf(ID, "%[^\0]s", last_commit);
        strcat(new_commit, "/");
        strcat(new_commit, last_commit);
        mkdir(new_commit, 0755);
        fclose(ID);

        char command[MAX_COMMAND_LENGTH] = "";
        sprintf(command, "cp -r %s/* %s", new_current, new_commit);
        system(command);

        char author_name[MAX_NAME_LENGTH] = "";
        FILE *name = fopen(".changiz/config/config_user_name", "r");
        fscanf(name, "%[^\0]s", author_name);
        fclose(name);

        char author_email[MAX_NAME_LENGTH] = "";
        FILE *email = fopen(".changiz/config/config_user_email", "r");
        fscanf(email, "%[^\0]s", author_email);
        fclose(email);

        time_t cur = time(NULL);
        char current_time[1000] = "";
        strcpy(current_time, ctime(&cur));

        char get_commit_count[MAX_NAME_LENGTH] = "";
        char get_comment[MAX_COMMIT_MESSAGE_LENGTH] = "";
        if (which_argv == 2)
        {
            char current_data[MAX_NAME_LENGTH] = ".changiz/data_saver/";
            strcat(current_data, temp_current_id);
            FILE *get_last_data = fopen(current_data, "r");
            fgets(get_comment, MAX_MESSAGE_LENGTH, get_last_data);
            fgets(get_comment, MAX_MESSAGE_LENGTH, get_last_data);
            fgets(get_commit_count, MAX_MESSAGE_LENGTH, get_last_data);
            fgets(get_commit_count, MAX_MESSAGE_LENGTH, get_last_data);
            fgets(get_commit_count, MAX_MESSAGE_LENGTH, get_last_data);
        }
        if (which_argv == 4)
        {
            sprintf(get_comment, "Comment: %s\n", argv[3]);
        }

        FILE *log = fopen(".changiz/log_file", "r");
        char last_log[100000] = "";
        fscanf(log, "%[^\0]s", last_log);
        fclose(log);

        FILE *creat_log = fopen(".changiz/log_file", "w");
        fprintf(creat_log, "Commit id: %s\n%sDate: %sAuthor: %s %s\n%sOn branch: %s\n\n", last_commit, get_comment, current_time, author_name, author_email, get_commit_count, branch);
        fprintf(creat_log, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\n");
        fclose(creat_log);

        char creat_data_file[MAX_FILENAME_LENGTH] = ".changiz/data_saver/";
        strcat(creat_data_file, last_commit);
        FILE *write_data = fopen(creat_data_file, "w");
        fprintf(write_data, "Commit id: %s\n%sDate: %sAuthor: %s %s\n%sOn branch: %s\n", last_commit, get_comment, current_time, author_name, author_email, get_commit_count, branch);
        fclose(write_data);

        FILE *complete_log = fopen(".changiz/log_file", "a");
        fprintf(complete_log, "%s", last_log);
        fclose(complete_log);

        FILE *ID_plus = fopen(".changiz/id_number", "w");
        int last_id = atoi(last_commit);
        last_id++;
        fprintf(ID_plus, "%d", last_id);

        FILE *add_current = fopen(".changiz/current_location", "w");
        fprintf(add_current, "%s", branch);
        fclose(add_current);

        FILE *add_commit_id = fopen(".changiz/current_commit_id", "w");
        fprintf(add_commit_id, "%s", last_commit);
        fclose(add_commit_id);

        FILE *add_branch = fopen(".changiz/branch_list", "a");
        fprintf(add_branch, "(%s) %s\n", last_commit, branch);
        fclose(add_branch);

        fprintf(stdout, "revert to commit %s done successfully", argv[which_argv]);
        return 1;
    }
}

int tag(int argc, char *const argv[])
{
    if (argc >= 4)
    {
        if (strcmp(argv[2], "-a") == 0)
        {
            char tag_name[MAX_NAME_LENGTH] = ".changiz/tags/";
            strcat(tag_name, argv[3]);

            char tag_name_data[MAX_MESSAGE_LENGTH] = "";
            sprintf(tag_name_data, "%s/data", tag_name);

            char tag_message[MAX_MESSAGE_LENGTH] = "";
            char tag_commit_id[MAX_NAME_LENGTH] = "";

            char author_name[MAX_NAME_LENGTH] = "";
            FILE *name = fopen(".changiz/config/config_user_name", "r");
            fscanf(name, "%[^\0]s", author_name);
            fclose(name);

            char author_email[MAX_NAME_LENGTH] = "";
            FILE *email = fopen(".changiz/config/config_user_email", "r");
            fscanf(email, "%[^\0]s", author_email);
            fclose(email);

            time_t cur = time(NULL);
            char current_time[1000] = "";
            strcpy(current_time, ctime(&cur));

            if (argc == 4)
            {
                struct dirent *same_tag_name = search_in_directory(argv[3], ".changiz/tags");
                if (same_tag_name == NULL)
                {
                    mkdir(tag_name, 0755);
                    FILE *tag_list = fopen(".changiz/tag_list", "a");
                    fprintf(tag_list, "%s\n", argv[3]);
                    fclose(tag_list);

                    FILE *commit_id = fopen(".changiz/current_commit_id", "r");
                    fscanf(commit_id, "%[^\0]s", tag_commit_id);
                    fclose(commit_id);

                    FILE *tag_file = fopen(tag_name_data, "w");
                    fprintf(tag_file, "tag: %s\ncommit: %s\nDate: %sAuthor: %s <%s>\nMessage: -\n", argv[3], tag_commit_id, current_time, author_name, author_email);
                    fclose(tag_file);

                    fprintf(stdout, "tag added successfully");
                    return 1;
                }
                else
                {
                    fprintf(stderr, "this tag-name already exist if you want to overwrite it use '-f");
                    return 0;
                }
            }
            if (argc == 9)
            {
                struct dirent *same_tag_name = search_in_directory(argv[3], ".changiz/tags");
                if (same_tag_name == NULL)
                {
                    mkdir(tag_name, 0755);
                    FILE *tag_list = fopen(".changiz/tag_list", "a");
                    fprintf(tag_list, "%s\n", argv[4]);
                    fclose(tag_list);
                }
                strcpy(tag_message, argv[5]);
                strcpy(tag_commit_id, argv[7]);

                FILE *tag_file = fopen(tag_name_data, "w");
                fprintf(tag_file, "tag: %s\ncommit: %s\nDate: %sAuthor: %s <%s>\nMessage: %s\n", argv[3], tag_commit_id, current_time, author_name, author_email, tag_message);
                fclose(tag_file);

                fprintf(stdout, "tag added successfully");
                return 1;
            }
            if (argc > 4 && argc < 9)
            {
                if (strcmp(argv[argc - 1], "-f") == 0)
                {
                    struct dirent *same_tag_name = search_in_directory(argv[3], ".changiz/tags");
                    if (same_tag_name == NULL)
                    {
                        mkdir(tag_name, 0755);
                        FILE *tag_list = fopen(".changiz/tag_list", "a");
                        fprintf(tag_list, "%s\n", argv[4]);
                        fclose(tag_list);
                    }
                    if (strcmp(argv[4], "-m") == 0)
                    {
                        strcpy(tag_message, argv[5]);
                        if (argc > 6)
                        {
                            if (strcmp(argv[6], "-c") == 0)
                            {
                                strcpy(tag_commit_id, argv[7]);
                            }
                            else
                            {
                                FILE *commit_id = fopen(".changiz/current_commit_id", "r");
                                fscanf(commit_id, "%[^\0]s", tag_commit_id);
                                fclose(commit_id);
                            }
                        }
                        else
                        {
                            FILE *commit_id = fopen(".changiz/current_commit_id", "r");
                            fscanf(commit_id, "%[^\0]s", tag_commit_id);
                            fclose(commit_id);
                        }
                    }
                    if (strcmp(argv[4], "-c") == 0)
                    {
                        strcpy(tag_commit_id, argv[5]);
                    }
                    FILE *tag_file = fopen(tag_name_data, "w");
                    fprintf(tag_file, "tag: %s\ncommit: %s\nDate: %sAuthor: %s <%s>\nMessage: %s\n", argv[3], tag_commit_id, current_time, author_name, author_email, tag_message);
                    fclose(tag_file);

                    fprintf(stdout, "tag added successfully");
                    return 1;
                }
                else
                {
                    struct dirent *same_tag_name = search_in_directory(argv[3], ".changiz/tags");
                    if (same_tag_name == NULL)
                    {
                        mkdir(tag_name, 0755);
                        FILE *tag_list = fopen(".changiz/tag_list", "a");
                        fprintf(tag_list, "%s\n", argv[4]);
                        fclose(tag_list);

                        if (strcmp(argv[4], "-m") == 0)
                        {
                            strcpy(tag_message, argv[5]);
                            if (argc > 6)
                            {
                                if (strcmp(argv[6], "-c") == 0)
                                {
                                    strcpy(tag_commit_id, argv[7]);
                                }
                                else
                                {
                                    FILE *commit_id = fopen(".changiz/current_commit_id", "r");
                                    fscanf(commit_id, "%[^\0]s", tag_commit_id);
                                    fclose(commit_id);
                                }
                            }
                            else
                            {
                                FILE *commit_id = fopen(".changiz/current_commit_id", "r");
                                fscanf(commit_id, "%[^\0]s", tag_commit_id);
                                fclose(commit_id);
                            }
                        }
                        if (strcmp(argv[4], "-c") == 0)
                        {
                            strcpy(tag_commit_id, argv[5]);
                        }
                        FILE *tag_file = fopen(tag_name_data, "w");
                        fprintf(tag_file, "tag: %s\ncommit: %s\nDate: %sAuthor: %s <%s>\nMessage: %s\n", argv[3], tag_commit_id, current_time, author_name, author_email, tag_message);
                        fclose(tag_file);

                        fprintf(stdout, "tag added successfully");
                        return 1;
                    }
                    else
                    {
                        fprintf(stderr, "this tag-name already exist if you want to overwrite it use '-f");
                        return 0;
                    }
                }
            }
        }
        if (strcmp(argv[2], "show") == 0)
        {
            char file_address[MAX_FILENAME_LENGTH] = "";
            sprintf(file_address, ".changiz/tags/%s/data", argv[3]);
            char command_cat[MAX_COMMAND_LENGTH] = "";
            sprintf(command_cat, "cat %s", file_address);
            system(command_cat);
        }
    }

    else if (argc == 2)
    {
        char save_tag[MAX_MESSAGE_LENGTH] = "";
        char save_line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
        char x;
        int line_count = 0;
        FILE *tag_list = fopen(".changiz/tag_list", "r");
        while (fgets(save_tag, 1000, tag_list))
        {
            strcpy(save_line[line_count], save_tag);
            line_count++;
        }
        rewind(tag_list);
        for (int i = 0; i < line_count; i++)
        {
            for (int j = 0; j < line_count; j++)
            {
                char temp[MAX_MESSAGE_LENGTH] = "";
                if (strcmp(save_line[i], save_line[j]) < 0)
                {
                    strcpy(temp, save_line[i]);
                    strcpy(save_line[i], save_line[j]);
                    strcpy(save_line[j], temp);
                }
            }
        }
        for (int i = 0; i < line_count; i++)
        {
            fprintf(stdout, "%s", save_line[i]);
        }
        return 1;
    }

    else
    {
        fprintf(stderr, "please enter a valid command");
        return 1;
    }
    return 0;
}

int grep(int argc, char *const argv[])
{
    char file_name[MAX_FILENAME_LENGTH] = "";
    strcpy(file_name, argv[3]);
    char word[MAX_COMMIT_MESSAGE_LENGTH] = "";
    strcpy(word, argv[5]);
    int exist_n = 0;
    if (argc > 6)
    {
        if (strcmp(argv[6], "-c") == 0)
        {
            char check_branch[MAX_FILENAME_LENGTH] = "";
            FILE *find_branch = fopen(".changiz/branch_list", "r");
            fscanf(find_branch, "%[^\0]s", check_branch);
            fclose(find_branch);

            char *find = strstr(check_branch, argv[7]);
            char branch[MAX_NAME_LENGTH] = "";
            sscanf(find + 3, "%s", branch);

            char file_address[MAX_NAME_LENGTH] = "";
            sprintf(file_address, ".changiz/branches/%s/%s/%s", branch, argv[7], argv[3]);

            if (strcmp(argv[8], "-n") == 0)
            {
                exist_n = 1;
            }

            FILE *open_file = fopen(file_address, "r");

            char line[MAX_LINE_LENGTH] = "";
            int counter_line = 0;
            while (fgets(line, 10000, open_file) != NULL)
            {
                counter_line++;
                char *find = strstr(line, argv[5]);
                if (find != NULL)
                {
                    char end[1000] = "";
                    char first[1000] = "";
                    char wordd[100] = "";
                    strcpy(end, find + strlen(argv[5]));
                    strncpy(wordd, find, strlen(argv[5]));
                    strncpy(first, line, find - line);

                    if (exist_n)
                    {
                        fprintf(stdout, "%d ->", counter_line);
                        fprintf(stdout, "%s" YELLOW "%s" COLOR_RESET "%s", first, wordd, end);
                        continue;
                    }
                    if (!exist_n)
                    {
                        fprintf(stdout, "%s" YELLOW "%s" COLOR_RESET "%s", first, wordd, end);
                        continue;
                    }
                }
            }
            fprintf(stdout, "Grep done successfully");
            return 1;
        }
        if (strcmp(argv[6], "-n") == 0 && argc == 7)
        {
            FILE *open_file = fopen(argv[3], "r");
            char line[MAX_LINE_LENGTH] = "";
            int counter_line = 0;
            while (fgets(line, 10000, open_file) != NULL)
            {
                counter_line++;
                char *find = strstr(line, argv[5]);
                if (find != NULL)
                {
                    char end[1000] = "";
                    char first[1000] = "";
                    char wordd[100] = "";
                    strcpy(end, find + strlen(argv[5]));
                    strncpy(wordd, find, strlen(argv[5]));
                    strncpy(first, line, find - line);
                    fprintf(stdout, ("%d ->"
                                     "%s" YELLOW "%s" COLOR_RESET "%s"),
                            counter_line, first, wordd, end);
                    continue;
                }
            }
            fprintf(stdout, "Grep done successfully");
            return 1;
        }
    }
    if (argc == 6)
    {
        FILE *open_file = fopen(argv[3], "r");
        char line[MAX_LINE_LENGTH] = "";
        while (fgets(line, 10000, open_file) != NULL)
        {
            char *find = strstr(line, argv[5]);
            if (find != NULL)
            {
                char end[1000] = "";
                char first[1000] = "";
                char wordd[100] = "";
                strcpy(end, find + strlen(argv[5]));
                strncpy(wordd, find, strlen(argv[5]));
                strncpy(first, line, find - line);

                fprintf(stdout, "%s" YELLOW "%s" COLOR_RESET "%s", first, wordd, end);
                continue;
            }
        }
        fprintf(stdout, "Grep done successfully");
        return 1;
    }
    return 1;
}

int diff(int argc, char *const argv[])
{
    char get_line1[MAX_LINE_LENGTH] = "";
    char get_line2[MAX_LINE_LENGTH] = "";
    char lines1[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    char lines2[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    char x, y;
    int i = 0, j = 0;
    int line_counter1 = 0, line_counter2 = 0;
    if (argc > 5)
    {
        FILE *f1 = fopen(argv[3], "r");
        while (fgets(get_line1, 1000, f1) != NULL)
        {
            line_counter1++;
            if (strcmp(get_line1, "\n") == 0)
            {
                continue;
            }
            if (get_line1[strlen(get_line1) - 1] == '\n')
            {
                get_line1[strlen(get_line1) - 1] = '\0';
            }
            if (line_counter1 >= atoi(argv[6]) || line_counter1 <= atoi(argv[7]))
            {
                strcpy(lines1[i], get_line1);
                i++;
            }
        }

        FILE *f2 = fopen(argv[4], "r");
        while (fgets(get_line2, 1000, f2) != NULL)
        {
            line_counter1++;
            if (strcmp(get_line2, "\n") == 0)
            {
                continue;
            }
            if (get_line2[strlen(get_line2) - 1] == '\n')
            {
                get_line2[strlen(get_line2) - 1] = '\0';
            }
            if (line_counter1 >= atoi(argv[9]) || line_counter1 <= atoi(argv[10]))
            {
                strcpy(lines2[j], get_line2);
                j++;
            }
        }

        int pluser1 = line_counter1 - i;
        int pluser2 = line_counter2 - j;
        if (i == j)
        {
            for (int a = 0; a < i; a++)
            {
                if (strcmp(lines1[a], lines2[a]) != 0)
                {
                    fprintf(stdout, "»»»»»»»»\n");
                    fprintf(stdout, "<%s> - <%d>\n", argv[3], a + pluser1);
                    fprintf(stdout, BLU "%s\n" COLOR_RESET, lines1[a]);
                    fprintf(stdout, "<%s> - <%d>\n", argv[4], a + pluser2);
                    fprintf(stdout, PURPLE "%s\n" COLOR_RESET, lines2[a]);
                    fprintf(stdout, "»»»»»»»»\n");
                }
            }
            return 1;
        }
        if (i > j)
        {
            for (int a = 0; a < i - j; a++)
            {
                if (strcmp(lines1[a], lines2[a]) != 0)
                {
                    fprintf(stdout, "»»»»»»»»\n");
                    fprintf(stdout, "<%s> - <%d>\n", argv[3], a + pluser1);
                    fprintf(stdout, BLU "%s\n" COLOR_RESET, lines1[a]);
                    fprintf(stdout, "<%s> - <%d>\n", argv[4], a + pluser2);
                    fprintf(stdout, PURPLE "%s\n" COLOR_RESET, lines2[a]);
                    fprintf(stdout, "»»»»»»»»\n");
                }
            }
            fprintf(stdout, "extra lines in file1:\n");
            for (int b = i - j; b < i; b++)
            {
                fprintf(stdout, BLU "%s\n" COLOR_RESET, lines1[b]);
            }
            return 1;
        }
        if (i < j)
        {
            for (int a = 0; a < j - i; a++)
            {
                if (strcmp(lines1[a], lines2[a]) != 0)
                {
                    fprintf(stdout, "»»»»»»»»\n");
                    fprintf(stdout, "<%s> - <%d>\n", argv[3], a + pluser1);
                    fprintf(stdout, BLU "%s\n" COLOR_RESET, lines1[a]);
                    fprintf(stdout, "<%s> - <%d>\n", argv[4], a + pluser2);
                    fprintf(stdout, PURPLE "%s\n" COLOR_RESET, lines2[a]);
                    fprintf(stdout, "»»»»»»»»\n");
                }
            }
            fprintf(stdout, "extra lines in file2:\n");
            for (int b = j - i; b < j; b++)
            {
                fprintf(stdout, PURPLE "%s\n" COLOR_RESET, lines2[b]);
            }
            return 1;
        }
    }
    FILE *f1 = fopen(argv[3], "r");
    FILE *f2 = fopen(argv[4], "r");
    while (fgets(get_line1, 1000, f1) != NULL && fgets(get_line2, 1000, f2) != NULL)
    {
        int is_ended = 0;

        line_counter1++;
        while (strcmp(get_line1, "\n") == 0)
        {
            line_counter1++;
            if (fgets(get_line1, 1000, f1) == NULL)
            {
                is_ended = 1;
                break;
            }
        }

        line_counter2++;
        while (strcmp(get_line2, "\n") == 0)
        {
            line_counter2++;
            if (fgets(get_line2, 1000, f2) == NULL)
            {
                is_ended = 1;
                break;
            }
        }

        if (is_ended)
        {
            break;
        }

        if (strcmp(get_line1, get_line2) != 0)
        {
            fprintf(stdout, "»»»»»»»»\n");
            fprintf(stdout, "<%s> - <%d>\n", argv[3], line_counter1);
            fprintf(stdout, BLU "%s\n" COLOR_RESET, get_line1);
            fprintf(stdout, "<%s> - <%d>\n", argv[4], line_counter2);
            fprintf(stdout, PURPLE "%s\n" COLOR_RESET, get_line2);
            fprintf(stdout, "»»»»»»»»\n");
        }
    }

    while (fgets(get_line1, 1000, f1) != NULL)
    {
        fprintf(stdout, BLU "file 1: %s\n" COLOR_RESET, get_line1);
    }

    while (fgets(get_line2, 1000, f2) != NULL)
    {
        fprintf(stdout, PURPLE "file 2: %s\n" COLOR_RESET, get_line2);
    }

    return 1;
}

// hooks
char file_size_check(char *file_address)
{
    FILE *fp = fopen(file_address, "r");
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    res = res / (1024 * 1024);
    if (res < 5)
    {
        return 'P';
    }
    return 'F';
}

char character_limit(char *file_address)
{
    char str[MAX_NAME_LENGTH] = "";
    if (strstr(file_address, ".c") != NULL || strstr(file_address, ".txt") != NULL)
    {
        FILE *fp = fopen(file_address, "r");
        fscanf(fp, "%[^\0]s", str);
        if (strlen(str) < 20000)
        {
            return 'P';
        }
        else
        {
            return 'F';
        }
    }
    else
    {
        return 'S';
    }
}

char eof_blank_space(char *file_address)
{
    char str[MAX_NAME_LENGTH] = "";
    if (strstr(file_address, ".c") != NULL || strstr(file_address, ".txt") != NULL)
    {
        FILE *fp = fopen(file_address, "r");
        fscanf(fp, "%[^\0]s", str);
        if ((str[strlen(str) - 1] == ' ') || (str[strlen(str) - 1] == '\t') || (str[strlen(str) - 1] == '\n'))
        {
            return 'F';
        }
        else
        {
            return 'P';
        }
    }
    else
    {
        return 'S';
    }
}

char format_check(char *file_address)
{
    char str[MAX_NAME_LENGTH] = "";
    if (strstr(file_address, ".c") != NULL || strstr(file_address, ".txt") != NULL || strstr(file_address, ".mp4") != NULL || strstr(file_address, ".mp3") != NULL || strstr(file_address, ".wav") != NULL || strstr(file_address, ".png") != NULL)
    {
        return 'P';
    }
    else
    {
        return 'F';
    }
}

char static_error_check(char *file_address)
{
    if (strstr(file_address, ".c") != NULL)
    {
        char command[MAX_COMMAND_LENGTH] = "";
        sprintf(command, "gcc %s -o test 2>/dev/null", file_address);
        if (system(command) == 0)
        {
            return 'P';
        }
        else
        {
            return 'F';
        }
    }
    else
    {
        return 'S';
    }
}

char todo_check(char *file_address)
{
    char str[MAX_NAME_LENGTH] = "";
    if (strstr(file_address, ".c") != NULL || strstr(file_address, ".txt") != NULL)
    {
        FILE *fp = fopen(file_address, "r");
        fscanf(fp, "%[^\0]s", str);
        if (strstr(str, "TODO") != NULL)
        {
            return 'P';
        }
        else
        {
            return 'F';
        }
    }
    else
    {
        return 'S';
    }
}

int brace_count(char *file_string, char brace)
{
    int counter = 0;
    for (int i = 0; i < strlen(file_string); i++)
    {
        if (file_string[i] = brace)
        {
            counter++;
        }
    }
    return counter;
}

char balance_braces(char *file_address)
{
    char str[MAX_NAME_LENGTH] = "";
    if (strstr(file_address, ".c") != NULL || strstr(file_address, ".txt") != NULL)
    {
        FILE *fp = fopen(file_address, "r");
        fscanf(fp, "%[^\0]s", str);
        if (brace_count(str, '[') == brace_count(str, ']') && brace_count(str, '{') == brace_count(str, '}') || brace_count(str, '(') == brace_count(str, ')'))
        {
            return 'P';
        }
        else
        {
            return 'F';
        }
    }
    else
    {
        return 'S';
    }
}

int pre_commit(int argc, char *const argv[])
{
    if (argc == 4 && strcmp(argv[3], "list") == 0)
    {
        fprintf(stdout, "•todo-check          •eof-blank-space      •format-check\n");
        fprintf(stdout, "•balance-braces      •indentation-check    •static-error-check\n");
        fprintf(stdout, "•file-size-check     •character-limit      •time-limit\n");
        return 1;
    }
    if (argc == 2)
    {
        char situation[100];
        char hook_id[100][100];
        char file_name[100];
        struct dirent *look_file;
        DIR *stage = opendir(".changiz/stage");
        while ((look_file = readdir(stage)) != NULL)
        {
            strcpy(file_name, look_file->d_name);
            if ((strcmp(look_file->d_name, ".") == 0) || (strcmp(look_file->d_name, "..") == 0) || (strcmp(look_file->d_name, ".DS_Store") == 0))
            {
                continue;
            }
            int i = 0;
            situation[i] = character_limit(look_file->d_name);
            strcpy(hook_id[i], "character-limit");
            i++;
            situation[i] = file_size_check(look_file->d_name);
            strcpy(hook_id[i], "file-size-check");
            i++;
            situation[i] = eof_blank_space(look_file->d_name);
            strcpy(hook_id[i], "eof-blank-space");
            i++;
            situation[i] = format_check(look_file->d_name);
            strcpy(hook_id[i], "format-check");
            i++;
            situation[i] = static_error_check(look_file->d_name);
            strcpy(hook_id[i], "static-error-check");
            i++;
            situation[i] = todo_check(look_file->d_name);
            strcpy(hook_id[i], "todo-check");
            i++;
            situation[i] = balance_braces(look_file->d_name);
            strcpy(hook_id[i], "balance-braces");
            i++;
            for (int j = 0; j < 7; j++)
            {
                if (j == 0)
                {
                    fprintf(stdout, "%s:\n", file_name);
                }
                fprintf(stdout, BLU "\"%s\"..................." COLOR_RESET, hook_id[j]);
                if (situation[j] == 'P')
                {
                    fprintf(stdout, GREEN "PASSED\n" COLOR_RESET);
                    continue;
                }
                if (situation[j] == 'F')
                {
                    fprintf(stdout, RED "FAILED\n" COLOR_RESET);
                    continue;
                }
                if (situation[j] == 'S')
                {
                    fprintf(stdout, YELLOW "SKIPPED\n" COLOR_RESET);
                    continue;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "init") == 0)
    {
        return init(argc, argv);
    }
    else if (strcmp(argv[1], "config") == 0)
    {
        return configs(argc, argv);
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        return add(argc, argv);
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        return reset(argc, argv);
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        return commit(argc, argv);
    }
    else if (strcmp(argv[1], "set") == 0)
    {
        return set(argc, argv);
    }
    else if (strcmp(argv[1], "replace") == 0)
    {
        return replace(argc, argv);
    }
    else if (strcmp(argv[1], "remove") == 0)
    {
        return remove_func(argc, argv);
    }
    else if (strcmp(argv[1], "branch") == 0)
    {
        return branch(argc, argv);
    }
    else if (strcmp(argv[1], "log") == 0)
    {
        return log_func(argc, argv);
    }
    else if (strcmp(argv[1], "checkout") == 0)
    {
        return checkout(argc, argv);
    }
    else if (strcmp(argv[1], "status") == 0)
    {
        return status(argc, argv);
    }
    else if (strcmp(argv[1], "revert") == 0)
    {
        return revert(argc, argv);
    }
    else if (strcmp(argv[1], "tag") == 0)
    {
        return tag(argc, argv);
    }
    else if (strcmp(argv[1], "grep") == 0)
    {
        return grep(argc, argv);
    }
    else if (strcmp(argv[1], "diff") == 0)
    {
        return diff(argc, argv);
    }
    else if (strcmp(argv[1], "pre-commit") == 0)
    {
        return pre_commit(argc, argv);
    }
    else
    {
        fprintf(stderr, "Please enter a valid command");
        return 1;
    }
}