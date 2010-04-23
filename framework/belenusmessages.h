//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : belenusmessages.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Uzenetek szerver es kliens kozott
//====================================================================================

#ifndef BELENUSMESSAGES_H
#define BELENUSMESSAGES_H

//====================================================================================
// Hibauzenetek
//------------------------------------------------------------------------------------
#define ERR_HOST_NOT_FOUND                      "_HOST_NOT_FOUND_"
#define ERR_CONNECTION_REJECTED                 "_CONNECTION_REJECTED_"
#define ERR_INVALID_CONNECTION_RECEIVED         "_INVALID_CONNECTION_RECEIVED_"
#define ERR_UNREGISTERED_ADMIN                  "_UNREGISTERED_ADMIN_"
#define ERR_UNREGISTERED_CLIENT                 "_UNREGISTERED_CLIENT_"
#define ERR_MESSAGE_LENGTH_INVALID              "_MESSAGE_LENGTH_INVALID_"
#define ERR_MESSAGE_CHEKCSUM_MISMATCH           "_MESSAGE_CHEKCSUM_MISMATCH_"
#define ERR_CLIENT_SERIAL_ALREADY_REGISTERED    "_CLIENT_SERIAL_ALREADY_REGISTERED_"
#define ERR_UNABLE_TO_REGISTER_CLIENT_SERIAL    "_UNABLE_TO_REGISTER_CLIENT_SERIAL_"
#define ERR_UNABLE_TO_DELETE_CLIENT_SERIAL      "_UNABLE_TO_DELETE_CLIENT_SERIAL_"
#define ERR_UNABLE_TO_PROCESS_SQL_STATEMENT     "_UNABLE_TO_PROCESS_SQL_STATEMENT_"

//====================================================================================
// Valaszuzenetek
//------------------------------------------------------------------------------------
#define INF_CONNECTION_CLOSED                   "_CONNECTION_CLOSED_"
#define INF_ADMIN_AUTHORIZED                    "_ADMIN_AUTHORIZED_"
#define INF_CLIENT_SERIAL_REGISTERED            "_CLIENT_SERIAL_REGISTERED_"
#define INF_CLIENT_SERIAL_DELETED               "_CLIENT_SERIAL_DELETED_"
#define INF_CLIENT_AUTHORIZED                   "_CLIENT_AUTHORIZED_"
#define INF_READY_TO_RECEIVE_SQL                "_READY_TO_RECEIVE_SQL_"
#define INF_SQL_RECEIVED                        "_SQL_RECEIVED_"
#define INF_SQL_STATEMENT_PROCESSED             "_SQL_STATEMENT_PROCESSED_"

//====================================================================================
// Uzenet tipusok
//------------------------------------------------------------------------------------
#define ACT_CONNECT                             "CON"
#define ACT_REQUEST_DATA                        "REQ"
#define ACT_SEND_INFORMATION                    "INF"
#define ACT_SEND_ERROR                          "ERR"
#define ACT_SEND_PASSWORD                       "PSW"
#define ACT_START_ARCHIVE_PROCESS               "ARC"
#define ACT_SEND_SQL_STATEMENT                  "SQL"
#define ACT_PROCESS_SQL_STATEMENT               "PRC"
#define ACT_DISCONNECT                          "BYE"

//====================================================================================
// Parancsuzenetek
//------------------------------------------------------------------------------------
#define REQ_ADMIN_REQUEST_LOGIN                 "_ADMIN_REQUEST_LOGIN_"
#define REQ_ADMIN_PASSWORD                      "_ADMIN_PASSWORD_"
#define REQ_REGISTER_CLIENT_SERIAL              "_REGISTER_CLIENT_SERIAL_"
#define REQ_DELETE_CLIENT_SERIAL                "_DELETE_CLIENT_SERIAL_"

//====================================================================================

#endif // BELENUSMESSAGES_H
