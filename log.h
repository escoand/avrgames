#ifndef LOGGER_H
#define LOGGER_H


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define LOGE(fmt, ...) logger("ERROR " __FILE__ ":" TOSTRING(__LINE__), fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) logger("WARN  " __FILE__ ":" TOSTRING(__LINE__), fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) logger("INFO  " __FILE__ ":" TOSTRING(__LINE__), fmt, ##__VA_ARGS__)
#ifdef DEBUG
#define LOGD(fmt, ...) logger("DEBUG " __FILE__ ":" TOSTRING(__LINE__), fmt, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...) ((void)0)
#endif 


void logger(const char* lvl, const char* fmt, ...);


#endif /* LOG_H */
