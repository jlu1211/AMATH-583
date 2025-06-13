from datetime import datetime, timedelta

def parse_stay_periods(file_path):
    """Parse the stay periods from the text file."""
    stay_periods = []
    with open(file_path, 'r') as file:
        for line in file:
            if '---' in line:
                # Extract dates from the line
                dates = line.split('---')
                start_date = dates[0].strip()
                end_date = dates[1].split('(')[0].strip()
                stay_periods.append((start_date, end_date))
    return stay_periods

def calculate_days_in_cycle(stay_periods, cycle_start_date):
    """Calculate total days stayed in a cycle year, with debug output."""
    cycle_end_date = cycle_start_date + timedelta(days=365)
    total_days = 0
    debug_info = []
    
    for start, end in stay_periods:
        start_date = datetime.strptime(start, "%Y-%m-%d")
        end_date = datetime.strptime(end, "%Y-%m-%d")
        
        # Only process stays that overlap with the cycle year
        if end_date >= cycle_start_date and start_date <= cycle_end_date:
            # Adjust dates to be within cycle year
            period_start = max(start_date, cycle_start_date)
            period_end = min(end_date, cycle_end_date)
            
            # Calculate days (inclusive of both start and end dates)
            days = (period_end - period_start).days + 1
            total_days += days
            debug_info.append((period_start.strftime('%Y-%m-%d'), period_end.strftime('%Y-%m-%d'), days))
    
    return total_days, debug_info

def main():
    # Read stay periods from file
    stay_periods = parse_stay_periods('US_B1.txt')
    
    # Get cycle start date from user
    while True:
        try:
            cycle_start = input("Enter your cycle start date (YYYY-MM-DD): ")
            cycle_start_date = datetime.strptime(cycle_start, "%Y-%m-%d")
            break
        except ValueError:
            print("Invalid date format. Please use YYYY-MM-DD format.")
    
    # Calculate total days and debug info
    total_days, debug_info = calculate_days_in_cycle(stay_periods, cycle_start_date)
    
    # Print results in Chinese
    print(f"\n周期年份: {cycle_start_date.strftime('%Y-%m-%d')} 至 {(cycle_start_date + timedelta(days=365)).strftime('%Y-%m-%d')}")
    print(f"在美国停留的总天数: {total_days} 天")
    print(f"周期内剩余天数: {180 - total_days} 天")
    
    if total_days > 180:
        print("\n警告: 您已超过此周期年份的180天限制!")
    elif total_days == 180:
        print("\n注意: 您已达到此周期年份的最大180天限制。")
    else:
        print("\n您在此周期年份内未超过180天限制。")
    
    # Print debug info
    print("\n计数期间的明细:")
    for start, end, days in debug_info:
        print(f"  {start} 至 {end}: {days} 天")

if __name__ == "__main__":
    main() 