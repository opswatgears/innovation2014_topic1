#ifndef DEFINITION_H
#define DEFINITION_H

#endif // DEFINITION_H

#define URL_METASCAN_ONLINE_UPLOAD_FILE             "https://scan.metascan-online.com/v2/file"
#define URL_METASCAN_ONLINE_GET_SCAN_FILE_STATUS    "https://%1/file/%2"
#define URL_METASCAN_ONLINE_GET_SCAN_FILE_RESULT    "https://www.metascan-online.com/en/scanresult/file/%1"
#define URL_METASCAN_ONLINE_LOOKUP_HASH             "http://hashlookup.metascan-online.com/v2/hash/%1"
#define URL_METASCAN_ONLINE_SCAN_IP_ADDRESS         "https://ipscan.metascan-online.com/v1/scan/%1"
#define URL_METASCAN_ONLINE_SCAN_RESULT_IMAGE       "https://www.metascan-online.com/badge/index.php?date=%1&engines=%2&virus=%3"


#define HTML_TEMPLATE_ENGINE                        "engine"
#define HTML_TEMPLATE_SCAN_PENDING                  "scan_pending"
#define HTML_TEMPLATE_SCAN_RESULT                   "scan_result"


#define SETTINGS_DOMAIN                             "opswat.com"
#define SETTINGS_USER                               "ble"
#define SETTINGS_MO_KEY                             "MO_KEY"

#define APP_NAME                                    "Metascan Online Uploader"
#define MSG_QUIT_APP                                "Are you sure you want to quit?"
#define MSG_EMPTY_KEY                               "Metascan key cannot be empty?"

#define DATE_TIME_FORMAT                            "yyyy-MM-dd'T'hh:mm:ss'Z'"
#define DATE_TIME_FORMAT_2                          "yyyy-MM-dd'T'hh:mm:sss'Z'"

#define DATABASE_NAME                               "MoUploaderDB"
#define DATABASE_PATH                               "MoUploaderDB.qdn"
#define TABLE_EVENTLOG                              "TBL_EVENTLOG"
#define COL_DATE                                    "COL_DATE"
#define COL_FILE_NAME                               "COL_FILE_NAME"
#define COL_STATUS                                  "COL_STATUS"
#define COL_MD5                                     "COL_MD5"
#define COL_LINK                                    "COL_LINK"

#define SCAN_FILE_STATUS_COMPLETED					"Scan completed"
#define SCAN_FILE_STATUS_PENDING 					"Scan pending..."
#define UPLOAD_FILE_STATUS_PENDING 					"Uploading..."
#define UPLOAD_FILE_STATUS_COMPLETED                "Upload completed"
#define UPLOAD_FILE_STATUS_NORMAL 					"Select your file for uploading"
#define FETCH_REPORT_STATUS_PENDING 				"Fetching report..."
#define STATUS_NOT_AVAILABLE						"Not available"

#define TIMER_FETCH_REPORT                          2000

#define STATE_QUEUED                                0
#define STATE_UPLOADING                             1
#define STATE_UPLOAD_COMPLETED                      2
#define STATE_FETCHING_REPORT                       3
#define STATE_SCAN_COMPLETED                        4

#define MAX_CONCURENT_UPLOAD                        100

#define MESSAGE_QUEUED                              "Queued"
#define MESSAGE_CALCULATE_MD5                       "Calculating MD5..."
#define UNKNOWN_STATUS                              "Unknown"

#define HTTP_RESPONSE_CODE                         "HTTP %1"

#define HTTP_MSG_ERROR_0                           "Network error."
#define HTTP_MSG_ERROR_400                         "Unsupported HTTP method or invalid http request (e.g., empty body)."
#define HTTP_MSG_ERROR_401                         "Either missing API key or invalid API is passed."
#define HTTP_MSG_ERROR_403                         "Either missing API key or invalid API is passed."
#define HTTP_MSG_ERROR_404                         "Not found."
#define HTTP_MSG_ERROR_500                         "Server temporarily unavailable. Try again later."

#define HASH_NOT_FOUND                             "NOT FOUND"

#define UNKNOWN_DATE                               "N/A"

#define MAIN_WINDOW_WIDTH                           832
#define MAIN_WINDOW_HEIGHT                          634
#define ABOUT_DIALOG_WIDTH                          674
#define ABOUT_DIALOG_HEIGHT                         451
#define SETTINGS_DIALOG_WIDTH                       341
#define SETTINGS_DIALOG_HEIGHT                      119
