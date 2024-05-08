import os
import shutil

ignore_files = {'.git', '.vscode', '.gitignore'}
remove_file_rule = ['cmake_install.cmake', 'CMakeCache.txt', 'CMakeFiles', 'Makefile', 'build']
remove_file_suffix = ['-bin']

del_count =0

def clean(dir_path):
    for file in os.listdir(dir_path):
        full_path = os.path.join(dir_path, file)

        if file in ignore_files:
            continue

        if os.path.isdir(full_path):
            clean(full_path)
            del_func = shutil.rmtree
        else:
            del_func = os.remove

        def delete_file():
            print(f'需要删除 {full_path}')
            del_count += 1
            del_func(full_path)

        for rule in remove_file_rule:
            if rule in file:
                delete_file()

        for suffix in remove_file_suffix:
            if file.endswith(suffix):
                delete_file()

print('开始清理')
clean(os.getcwd())
print(f'清理完毕，共删除 {del_count} 个文件')
