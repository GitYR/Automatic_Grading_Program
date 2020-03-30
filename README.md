# 2019_lsp_project1

It was the first linux system programming project in first semester of 2019.
The function of this project is to score student's midterm exam with the right answer files. 

There are two main functions. 

The first function is to score student's blank answers with the right blank answers. This function was written by 2019 SoongSil University Operating System Lab.(I don't know who wrote exactly,but must be the one of the 2019 semester's assistants)

The second function is to score student's codes with the answer codes. Student's codes have to be run within 5 seconds. If there are any compile errors,the result point would be zero. In warning cases, the program subtracts 0.5 points for every warning messages. The function was written by Choi Young Rim(me).

If you want to execute this program, at first, clone my git address and do 'make' command to build an executable file.
If you did above recommendations, do 'sudo ./ssu_score -h' to know how to use the program and if you are well-acquainted with the former informations, do 'sudo ./ssu_score [STUDENT_DIRECTORY] [ANSWER_DIRECTORY] [OPTIONS]'
