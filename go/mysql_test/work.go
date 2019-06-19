package main

import (
	"fmt"
	"strconv"
	"sync"
	"time"
)

func clear(){
	if re, err := db.Exec("truncate table user"); err != nil {
		fmt.Printf("re:%v, err:%v", re, err)
	}
}

func way1(){
	insertStart1 := time.Now()
	for i := 1;i<=TIMES;i++{
		db.Exec("INSERT INTO user(id, name,age, sex) values(?,?,?,?)",i,"user"+strconv.Itoa(i),i, 0)
	}
	insertEnd1 := time.Now()
	//fmt.Println("方式1 insert total time:",insertEnd1.Sub(insertStart1).Seconds(), "ms.")
	fmt.Println("方式1 insert single time:", insertEnd1.Sub(insertStart1).Seconds() * 1000 / TIMES, "ms.")
}

func way2() {
	insertStart2 := time.Now()
	stm,_ := db.Prepare("INSERT INTO user(id, name,age, sex) values(?,?,?,?)")
	for i := 1;i<=TIMES;i++{
		stm.Exec(i,"user"+strconv.Itoa(i),i, 0)
	}
	insertEnd2 := time.Now()
	fmt.Println("方式2 insert single time:", insertEnd2.Sub(insertStart2).Seconds() * 1000 / TIMES)
}

func way3(){
	fmt.Println("---txn ALL ONCE----")
	insertStart3 := time.Now()
	tx,_ := db.Begin()
	for i := 1;i<=TIMES;i++{
		tx.Exec("INSERT INTO user(id, name,age, sex) values(?,?,?,?)",i,"user"+strconv.Itoa(i),i, 0)
	}
	tx.Commit()
	insertEnd3 := time.Now()
	fmt.Println("txn all:", insertEnd3.Sub(insertStart3), "insert single time:", insertEnd3.Sub(insertStart3)/TIMES)
}

func way3Single(){
	fmt.Println("---txn ", TIMES/EVERYTIMES, " time SINGLE GOROUTINE----")
	insertStart3 := time.Now()
	for j := 0; j < (TIMES/EVERYTIMES); j++ {
		tx, _ := db.Begin()
		for i := 1; i <= EVERYTIMES; i++ {
			count := j*EVERYTIMES + i
			tx.Exec("INSERT INTO user(id, name,age, sex) values(?,?,?,?)", count, "user"+strconv.Itoa(count), count, 0)
		}
		tx.Commit()
	}
	insertEnd3 := time.Now()
	fmt.Println("txn all:", insertEnd3.Sub(insertStart3), "insert single time:", insertEnd3.Sub(insertStart3)/ (TIMES/EVERYTIMES))
}

func way3Mutli(){
	fmt.Println("---txn ", TIMES/EVERYTIMES, " time Multi GOROUTINE----")
	var wg sync.WaitGroup
	insertStart3 := time.Now()
	for j := 0; j < (TIMES/EVERYTIMES); j++ {
		wg.Add(1)
		go func(j int) {
			defer wg.Done()
			tx, _ := db.Begin()
			for i := 1; i <= EVERYTIMES; i++ {
				count := j*EVERYTIMES + i
				tx.Exec("INSERT INTO user(id, name,age, sex) values(?,?,?,?)", count, "user"+strconv.Itoa(count), count, 0)
			}
			tx.Commit()
		}(j)
	}
	wg.Wait()
	insertEnd3 := time.Now()
	fmt.Println("txn all:", insertEnd3.Sub(insertStart3), "insert single time:",  insertEnd3.Sub(insertStart3)/ (TIMES/EVERYTIMES))
}


func way4(){
	fmt.Println("---bulk ", TIMES/EVERYTIMES, "time single GOROUTINE-----")
	insertStart3 := time.Now()
	for j:= 0; j < (TIMES/EVERYTIMES); j++ {
		sqlStr := "INSERT INTO user(id, name,age, sex) values"
		vals := []interface{}{}

		for i := 1; i <= EVERYTIMES; i++ {
			count := j*EVERYTIMES + i
			sqlStr += "(?,?,?,?),"
			vals = append(vals, count, "user"+strconv.Itoa(count), strconv.Itoa(count), "0")
		}


		sqlStr = sqlStr[0:len(sqlStr)-1]
		stmt, err := db.Prepare(sqlStr)
		if err != nil {
			fmt.Println("error:", err)
			return
		}
		if re, err := stmt.Exec(vals...); err != nil {
			fmt.Printf("stmt failed, re:%v, err:%v", re, err)
			return
		}
	}
	insertEnd3 := time.Now()
	fmt.Println("use time:", insertEnd3.Sub(insertStart3), "eve(100 count/ms):",  insertEnd3.Sub(insertStart3)/ (TIMES/EVERYTIMES))
}

func way4Multi(){
	fmt.Println("----bulk ", TIMES/EVERYTIMES, "multi GOROUTINE----")
	var wg sync.WaitGroup
	insertStart3 := time.Now()
	for j:= 0; j < (TIMES/EVERYTIMES); j++ {
		sqlStr := "INSERT INTO user(id, name,age, sex) values"
		vals := []interface{}{}

		for i := 1; i <= EVERYTIMES; i++ {
			count := j*EVERYTIMES + i
			sqlStr += "(?,?,?,?),"
			vals = append(vals, count, "user"+strconv.Itoa(count), strconv.Itoa(count), "0")
		}


		sqlStr = sqlStr[0:len(sqlStr)-1]
		wg.Add(1)
		go func (sqlStr string, vals []interface{}) {
			defer wg.Done()
			stmt, err := db.Prepare(sqlStr)
			if err != nil {
				fmt.Println("error:", err)
				return
			}
			stmt.Exec(vals...)
		}(sqlStr, vals)
	}
	wg.Wait()
	insertEnd3 := time.Now()
	fmt.Println("use time:", insertEnd3.Sub(insertStart3),"eve(100 count/ms):",  insertEnd3.Sub(insertStart3)/ (TIMES/EVERYTIMES))
}