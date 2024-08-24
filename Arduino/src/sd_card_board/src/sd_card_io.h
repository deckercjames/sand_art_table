
#ifndef SD_CARD_IO__
#define SD_CARD_IO__

bool init_sd_card();

bool open_next_file();

bool open_file_idx(int file_idx);

void close_current_file();

void get_next_line(char *buf, unsigned int buf_size);

bool file_completed();

#endif // SD_CARD_IO__
