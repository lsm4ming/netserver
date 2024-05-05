const fs = require('fs');
const path = require('path');

let delCount = 0;

const ignoreFiles = new Set(['.git', '.vscode', '.gitignore']);
const removeFileRule = ['cmake_install.cmake', 'CMakeCache.txt', 'CMakeFiles' , 'Makefile'];

const clean = (dirPath) => {
    // 读取目录中的所有文件和子目录
    fs.readdirSync(dirPath).forEach(file => {
        // 获取文件/子目录的完整路径
        const fullPath = path.join(dirPath, file);

        // 是否是忽略文件
        if (ignoreFiles.has(file)) {
            return;
        }

        let delFunc = fs.unlinkSync;

        // 检查该路径是文件还是目录
        const isDir = fs.statSync(fullPath).isDirectory()
        if (isDir) {
            // 递归遍历该目录
            clean(fullPath);
            delFunc = fs.rmdirSync;
        }

        // 是否符合删除规则
        removeFileRule.forEach(rule => {
            const regex = new RegExp(rule)
            if (regex.test(file)) {
                console.log(`需要删除${fullPath}`)
                delCount++;
                delFunc(fullPath, { recursive: true });
            }
        })
    });
}

console.log('开始清理')
clean(process.cwd())
console.log(`清理完毕，共删除${delCount}个文件`)