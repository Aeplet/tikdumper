extern storage_device_t current_device;

void clear_console();
void full_screen_reset();
void print_top();
void move_left_right(u8 direction);
void device_select_menu();
storage_device_t get_device_menu_selection();