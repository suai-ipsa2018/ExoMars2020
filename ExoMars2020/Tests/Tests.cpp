#include <iostream>
#include <systemc.h>
#include <sqlite3.h>

int sc_main(int argc, char* argv[])
{
	sqlite3* db;
	char* zErrMsg = nullptr;
	int rc;

	rc = sqlite3_open("test.db", &db);

	if (rc)
	{
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		return 1;
	}
	else
		std::cout << "Opened database successfully" << std::endl;
	rc = sqlite3_exec(db, R"sql(
DROP TABLE IF EXISTS PrintUnit;

CREATE TABLE PrintUnit(
ID INT PRIMARY     KEY    NOT NULL,
RECEIVER_ADDRESS   INT    NOT NULL,
SENDER_ADDRESS     INT    NOT NULL,
SIZE               INT    NOT NULL,
CRC                INT    NOT NULL
);

INSERT INTO PrintUnit VALUES (0, 38, 32, 45, 50000);
INSERT INTO PrintUnit VALUES (1, 38, 32, 25, 151);
INSERT INTO PrintUnit VALUES (2, 38, 32, 27, 1515);
)sql", [](void *NotUsed, int argc, char **argv, char **azColName)->int { return 0; }, nullptr, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		std::cout << "Table created successfully" << std::endl;
	}


	rc = sqlite3_exec(db, R"sql(
	SELECT * FROM PrintUnit;
)sql", [](void *NotUsed, int argc, char **argv, char **azColName) -> int {
		int i;
		for (i = 0; i < argc; i++) {
			std::cout << azColName[i] << " = " << argv[i] << '\t' << sc_uint<16>(argv[i]).to_string(SC_BIN_US) << std::endl;;
		}
		std::cout << std::endl;
		return 0;
	}
	, nullptr, &zErrMsg);


	sqlite3_close(db);

	return 0;
}

