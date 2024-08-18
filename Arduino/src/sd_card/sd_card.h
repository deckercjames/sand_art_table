
#ifndef SD_CARD_H__
#define SD_CARD_H__

bool init_sd_card();

bool open_next_file();

bool file_completed();

void get_next_line(char *buf, unsigned int buf_size);

void close_current_file();

#endif //SD_CARD
