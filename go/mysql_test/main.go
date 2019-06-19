package main

import (
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
)
const (
	USERNAME = "root"
	//PASSWORD = "*******"
	NETWORK  = "tcp"
	SERVER   = "localhost"
	PORT     = 3306
	DATABASE = "test"
	TIMES    = 10000
	EVERYTIMES = 100
)

var db = &sql.DB{}

func main(){
	dsn := fmt.Sprintf("%s@%s(%s:%d)/%s",USERNAME,NETWORK,SERVER,PORT,DATABASE)
	db,_ = sql.Open("mysql", dsn)

	clear()
	way4()
	clear()
	way4Multi()
	clear()
	way3()
	clear()
	way3Single()
	clear()
	way3Mutli()
}