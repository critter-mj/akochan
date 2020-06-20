#!/bin/sh

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
mjai server --port=11602 --game_type=tonpu --room=default --games=5 --log_dir='./mjai_result' './system.exe mjai_client 11602 setup_mjai.json' mjai-manue mjai-manue mjai-manue

