#!/usr/bin/env bash

# tool for creating, restoring, managing, & tracking dir backups

backup_dir="$HOME/my_backups"
log_file="$HOME/activity.log"

if [ ! -d "$backup_dir" ]; then  # create dir
    if ! mkdir -p "$backup_dir"; then
        echo "Error: Backup directory not created."
        exit 1
    fi
fi

# record activity
log_activity() {
    local message="$1"
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $message" >> "$log_file"
}

# create compressed backup
create_backup() {
    echo -e "\nCreate New Backup :)\nAvailable disk space:"
    df -h . | awk 'NR == 1 || NR == 2'
    echo ""
    echo -n "Enter absolute path of directory to back up: "
    read -r source_dir
    if [ -z "$source_dir" ]; then
        echo "Error: Path can't be empty."
        return
    fi
    if [ ! -d "$source_dir" ]; then
        echo "Error: '$source_dir' doesn't exist."
        log_activity "Backup failed: '$source_dir' not found."
        return
    fi
    # generate backup filename
    local timestamp
    local dir_name
    local parent_dir
    local backup_filename
    local backup_path
    timestamp=$(date '+%Y%m%d_%H%M%S')
    source_dir="${source_dir%/}" # remove trailing / if user enters
    dir_name=$(basename "$source_dir")
    parent_dir=$(dirname "$source_dir")
    backup_filename="${dir_name}_backup_${timestamp}.tar.gz"
    backup_path="$backup_dir/$backup_filename"
    echo -e "\nCreating backup...\nSource: $source_dir\nBackup: $backup_path"
    if tar -czf "$backup_path" \
        -C "$parent_dir" \
        "$dir_name" 2>/dev/null; then
        echo "Success"
        log_activity "Created backup: $backup_filename from $source_dir"
    else
        echo -e "Error: Backup creation failed.\nPlease check directory permissions and available storage."
        log_activity "Backup failed: Couldn't archive $source_dir"
        # remove incomplete backup file if created
        if [ -f "$backup_path" ]; then 
            rm -f "$backup_path"
        fi
    fi
}

# store backup files
get_backups() {
    local -n backup_list=$1
    backup_list=()
    for file in "$backup_dir"/*.tar.gz; do
        if [ -f "$file" ]; then
            backup_list+=("$file")
        fi
    done
}

# display backups with #d selections
display_backups() {
    local -n backup_list=$1
    local index
    for index in "${!backup_list[@]}"; do
        echo "$((index + 1))) $(basename "${backup_list[$index]}")"
    done
}

# restore backup
restore_backup() {
    echo -e "\nRestore Backup?"
    local backups=()
    get_backups backups
    if [ "${#backups[@]}" -eq 0 ]; then
        echo "No backup files in $backup_dir."
        return
    fi
    echo -e "\nAvailable backups:"
    display_backups backups
    echo ""
    echo -n "Select number of backup to restore: "
    read -r selection
    # check if selection is valid #
    if ! [[ "$selection" =~ ^[0-9]+$ ]]; then
        echo "Invalid: Please enter a number."
        return
    fi
    # validate selection range
    if [ "$selection" -lt 1 ] ||
       [ "$selection" -gt "${#backups[@]}" ]; then
        echo "Invalid: Please choose a listed backup."
        return
    fi
    local selected_file="${backups[$((selection - 1))]}"
    echo ""
    echo -n "Enter absolute destination path: "
    read -r dest_dir
    if [ -z "$dest_dir" ]; then
        echo "Error: Path can't be empty."
        return
    fi
    # create destination if missing
    if [ ! -d "$dest_dir" ]; then
        echo -e "Destination directory doesn't exist.\nCreating '$dest_dir'..."
        if ! mkdir -p "$dest_dir"; then
            echo "Error: Unable to create."
            log_activity "Restore failed: Destination not created $dest_dir"
            return
        fi
    fi
    # warn user if not empty
    if [ -n "$(find "$dest_dir" -mindepth 1 -maxdepth 1 -print -quit 2>/dev/null)" ]; then
        echo -e "\nWarning: Destination directory isn't empty.\nExisting files with matching names may be overwritten."
        echo -n "Do you want to continue? (y/n): "
        read -r confirmation
        case "$confirmation" in
            y|Y|yes|YES)
                ;;
            *)
                echo "Restoration cancelled."
                log_activity "Restore cancelled: $(basename "$selected_file")"
                return
                ;;
        esac
    fi
    echo -e "\nRestoring backup to '$dest_dir'..."
    # extract selected archive
    if tar -xzf "$selected_file" -C "$dest_dir"; then
	echo "Success :)"
        log_activity "Restored backup: $(basename "$selected_file") to $dest_dir"
    else
        echo "Error: Restoration failed."
        log_activity "Restore failed: Could not extract $(basename "$selected_file")"
    fi
}

# display stored archives
view_history() {
    echo -e "\nBackup History..."
    local backups=()
    get_backups backups
    if [ "${#backups[@]}" -eq 0 ]; then
        echo "No backups created."
        return
    fi
    echo ""
    local index
    for index in "${!backups[@]}"; do
        local backup_file="${backups[$index]}"
        local backup_size
        backup_size=$(du -h "$backup_file" | awk '{print $1}')
        echo -e "$((index + 1))) $(basename "$backup_file")\n   Size: $backup_size"
    done
}

# delete backup
delete_backup() {
    echo -e "\nDelete Existing Backup?"
    local backups=()
    get_backups backups
    # check if archives exist
    if [ "${#backups[@]}" -eq 0 ]; then
        echo "No backup files available to delete."
        return
    fi
    echo -e "\nAvailable backups:"
    display_backups backups
    echo ""
    echo -n "Select number of backup to delete: "
    read -r selection
    # validate numeric input
    if ! [[ "$selection" =~ ^[0-9]+$ ]]; then
        echo "Invalid: Please enter a number."
        return
    fi
    # validate selection range
    if [ "$selection" -lt 1 ] ||
       [ "$selection" -gt "${#backups[@]}" ]; then
        echo "Invalid: Please choose a listed backup."
        return
    fi
    local selected_file="${backups[$((selection - 1))]}"
    echo -e "\nSelected backup: $(basename "$selected_file")"
    echo -n "Are you sure you want to delete it? (y/n): "
    read -r confirmation
    case "$confirmation" in
        y|Y|yes|YES)
            if rm "$selected_file"; then
                echo "Deleted"
                log_activity "Deleted backup: $(basename "$selected_file")"
            else
                echo "Error: Failed to delete backup."
                log_activity "Delete failed: Could not remove $(basename "$selected_file")"
            fi
            ;;
        *)
            echo "Backup deletion cancelled."
            ;;
    esac
}

# allows user to view/clear activity log
view_clear_log() {
    echo -e "\nActivity Log Management :)\n1) View log\n2) Clear log\n3) Return to main menu\n"
    echo -n "Choose an option: "
    read -r log_choice
    case "$log_choice" in
        1)
            echo -e "\nActivity Log..."
            if [ -f "$log_file" ] && [ -s "$log_file" ]; then
                cat "$log_file"
            else
                echo "Log is empty."
            fi
            ;;
        2)
            echo -n "Are you sure you want to clear log? (y/n): "
            read -r confirmation
            case "$confirmation" in
                y|Y|yes|YES)
                    if > "$log_file"; then
                        echo "Log cleared."
                    else
                        echo "Error: Log not cleared."
                    fi
                    ;;
                *)
                    echo "Log clearing cancelled."
                    ;;
            esac
            ;;
        3)
            return
            ;;
        *)
            echo "Invalid Choice: Returning to main menu."
            ;;
    esac
}

# main app
while true; do
    echo -e "\nBACKUP & RECOVERY MANAGEMENT :)\n"
    echo -e "1) Create backup\n2) Restore backup\n3) View backup history\n4) Delete existing backup\n5) View/clear log\n6) Exit program\n"
    echo -n "Please select an option (1-6): "
    read -r menu_option
    case "$menu_option" in
        1)
            create_backup
            ;;
        2)
            restore_backup
            ;;
        3)
            view_history
            ;;
        4)
            delete_backup
            ;;
        5)
            view_clear_log
            ;;
        6)
            echo -e "Exiting program...\nGoodbye!"
            log_activity "Program exited normally."
            exit 0
            ;;
        *)
	    echo "Invalid: Please enter number (1-6)."
            ;;
    esac
done

