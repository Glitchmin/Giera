#pragma once
enum class FileModeTypes {
	READ_ONLY, //Open a file for reading. The file must exist.
	WRITE_ONLY, //Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	APPEND, //Append to a file.Writing operations append data at the end of the file.The file is created if it does not exist.
	READ_WRITE //Open a file for update both reading and writing. The file must exist.
};