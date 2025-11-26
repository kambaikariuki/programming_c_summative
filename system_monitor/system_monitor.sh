#!/bin/bash

LOG_FILE="system_monitor.log"
CPU_THRESHOLD=80
MEM_THRESHOLD=80
DISK_THRESHOLD=80
SLEEP_INTERVAL=1     # seconds

# HELPER FUNCTIONS

# Ensure required commands exist
check_dependencies() {
    for cmd in top free df ps awk grep date; do
        if ! command -v $cmd >/dev/null 2>&1; then
            echo "Error: Required command '$cmd' not found." | tee -a "$LOG_FILE"
            exit 1
        fi
    done
}

# Timestamp helper
timestamp() {
    date +"%Y-%m-%d %H:%M:%S"
}

# Log message
log_message() {
    echo "$(timestamp) - $1" >> "$LOG_FILE"
}

# Get CPU usage
get_cpu_usage() {
    # Extract CPU idle from top, then compute usage
    top -bn1 | grep "Cpu(s)" | awk '{print 100 - $8}'
}

get_memory_usage() {
    # Memory usage percentage
    free | awk '/Mem:/ {printf("%.2f", $3/$2*100)}'
}

get_disk_usage() {
    # Disk usage for root filesystem
    df / | awk 'NR==2 {print $5}' | sed 's/%//'
}

view_system_status() {
    cpu=$(get_cpu_usage)
    mem=$(get_memory_usage)
    disk=$(get_disk_usage)

    echo "------ SYSTEM STATUS ------"
    echo "CPU Usage:    $cpu %"
    echo "Memory Usage: $mem %"
    echo "Disk Usage:   $disk %"
    echo "---------------------------"

    log_message "Status checked — CPU: $cpu%, MEM: $mem%, DISK: $disk%"
}

# Check CPU usage and print to console
check_thresholds() {
    cpu=$(get_cpu_usage)
    mem=$(get_memory_usage)
    disk=$(get_disk_usage)

    echo "------ System Status at $(timestamp) ------"
    log_message "Status checked — CPU: $cpu%, MEM: $mem%, DISK: $disk%"
    echo " "
    echo "CPU Usage:    $cpu %"
    echo "Memory Usage: $mem %"
    echo "Disk Usage:   $disk %"
    echo " "

    if (( $(echo "$cpu > $CPU_THRESHOLD" | bc -l) )); then
        log_message "ALERT: CPU usage high: $cpu% (threshold $CPU_THRESHOLD%)"
        echo "⚠ HIGH CPU ALERT: $cpu%"
    fi

    if (( $(echo "$mem > $MEM_THRESHOLD" | bc -l) )); then
        log_message "ALERT: Memory usage high: $mem% (threshold $MEM_THRESHOLD%)"
        echo "⚠ HIGH MEMORY ALERT: $mem%"
    fi

    if (( $(echo "$disk > $DISK_THRESHOLD" | bc -l) )); then
        log_message "ALERT: Disk usage high: $disk% (threshold $DISK_THRESHOLD%)"
        echo "⚠ HIGH DISK ALERT: $disk%"
    fi
}


monitor_loop() {
    echo "Monitoring system every $SLEEP_INTERVAL seconds..."
    echo "Press q then Enter to stop."
    log_message "Started monitoring loop."

    while true; do
        check_thresholds
        sleep "$SLEEP_INTERVAL"
        read -t 1 -p "" input 
        if [[ $input == "q" ]]; then
          log_message "Stopped monitoring loop"
          break
        fi
    done
}

# MENU / USER INTERFACE

set_thresholds() {
    echo "Set CPU threshold (%):"
    read cpu
    [[ "$cpu" =~ ^[0-9]+$ ]] && CPU_THRESHOLD=$cpu || echo "Invalid input"

    echo "Set Memory threshold (%):"
    read mem
    [[ "$mem" =~ ^[0-9]+$ ]] && MEM_THRESHOLD=$mem || echo "Invalid input"

    echo "Set Disk threshold (%):"
    read disk
    [[ "$disk" =~ ^[0-9]+$ ]] && DISK_THRESHOLD=$disk || echo "Invalid input"

    log_message "Thresholds updated — CPU:$CPU_THRESHOLD, MEM:$MEM_THRESHOLD, DISK:$DISK_THRESHOLD"
}

view_logs() {
    echo "------ LOG FILE CONTENTS ------"
    cat "$LOG_FILE"
    echo "--------------------------------"
}

clear_logs() {
    > "$LOG_FILE"
    echo "Logs cleared."
    log_message "Log file cleared."
}
menu() {
    while true; do
        echo ""
        echo "===== SYSTEM MONITOR MENU ====="
        echo "1) View system status"
        echo "2) Set alert thresholds"
        echo "3) View logs"
        echo "4) Clear logs"
        echo "5) Start monitoring loop"
        echo "6) Exit"
        echo ""
        echo "================================"
        echo ""
        echo -n "Select an option: "
        read choice

        case "$choice" in
            1) view_system_status ;;
            2) set_thresholds ;;
            3) view_logs ;;
            4) clear_logs ;;
            5) monitor_loop ;;
            6) echo "Goodbye!"; exit 0 ;;
            *) echo "Invalid option. Try again." ;;
        esac
    done
}

#              MAIN

check_dependencies
menu
