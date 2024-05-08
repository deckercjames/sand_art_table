
#ifndef SD_CARD_H__
#define SD_CARD_H__

bool init_sd_card();

void open_next_file();

void get_next_line(char *buf, unsigned int buf_size);
void get_test_line(char *buf, unsigned int buf_size);

#endif //SD_CARD
