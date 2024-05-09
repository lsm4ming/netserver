package main

import (
	"fmt"
	"log"
	"os"
	"time"
)

func writeLog(filePath string, content string) {
	file, err := os.OpenFile(filePath, os.O_APPEND|os.O_CREATE|os.O_WRONLY, os.ModePerm)
	if err != nil {
		log.Fatalf("无法打开文件: %v", err)
	}
	defer file.Close()
	file.WriteString(content)
}

func main() {
	fmt.Println(os.Args)
	for {
		writeLog("demo.log", fmt.Sprintf("[%s]-%s\n", time.Now().Format(time.DateTime), "hello"))
		time.Sleep(1 * time.Second)
	}
}
