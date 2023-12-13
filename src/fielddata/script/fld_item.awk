#=============================================================================================
#
#	フィールドアイテムスクリプトコンバータ
#
#	_EVENT_DATA記述をfld_item.evに出力する
#
#	フラグ定義をfld_item_flag.hに出力する
#
#	05.11.04 Satoshi Nohara

#	07.06.04 プラチナ用にオフセットを変更
#
#=============================================================================================
BEGIN{
	#=================================================================
	#
	#アイテム定義ファイル読み込み
	#
	#=================================================================
	ITEMSYM_H = "../../../include/itemtool/itemsym.h"

	#getline = １行読み込み
	#１は戻り値
	while ( 1 == getline < ITEMSYM_H ) {
		if ( NF == 7 && $1 =="#define" ) {
			ItemName[$7] = $2
		}
	}
	close(ITEMSYM_H)

	#書き込むファイル
	file1 = "fld_item.ev"		#スクリプト
	#file2 = "fld_item_flag.h"	#フラグ

	print "//====================================================================" > file1
	print "//						フィールドアイテム" >> file1
	print "//" >> file1
	print "//	●フィールドアイテムかんりひょう" >> file1
	print "//		fld_item.xls" >> file1
	print "//" >> file1
	print "//	●コンバート実行" >> file1
	print "//		fld_item(.bat)" >> file1
	print "//" >> file1
	print "//	●更新されるファイル" >> file1
	print "//		fld_item.xls" >> file1
	print "//		fld_item.ev" >> file1
	print "//		fld_item_def.h" >> file1
	print "//" >> file1
	print "//====================================================================" >> file1
	print "" >> file1
	print "\t.text" >> file1
	print "" >> file1
	print "\t.include	\"scr_seq_def.h\"" >> file1
	print "\t.include	\"../../../include/msgdata/msg_fld_item.h\"" >> file1
	print "" >> file1
	print "//--------------------------------------------------------------------" >> file1
	print "//						スクリプト本体" >> file1
	print "//--------------------------------------------------------------------" >> file1
	print "" >> file1
	print "//--------------------------------------------------------------------" >> file1
	print "//						スクリプトテーブル" >> file1
	print "//--------------------------------------------------------------------" >> file1
}

NR >= 4{

	#項目数チェック
	if( NF < 6 ) next

	#-----------------------------------------------------------------
	#
	#フラグマックスを超えていたらエラー(07/06/04 MAX = 328 )
	#
	#-----------------------------------------------------------------
	if( $6 > 327 ){
		printf("フラグの最大数を超えています！\n") >> "/dev/stderr"
		exit				#途中で終了
	}

	#-----------------------------------------------------------------
	#
	#同じ番号(フラグナンバーのオフセット)を使用してないかチェック
	#
	#-----------------------------------------------------------------
	if ( $6 in ID_List ) {

		#同じ数値を定義している個所がある場合
		printf("ERROR! 同じ値の定義があります\n") >> "/dev/stderr"
		printf("[%s] [%s] フラグ%s\n",ID_List[$6],$1,$6) >> "/dev/stderr"

		exit				#途中で終了
	}

	ID_List[$6] = $1		#要素として登録

	#-----------------------------------------------------------------
	#
	#
	#
	#-----------------------------------------------------------------
	if( $1 == "マップ名" )	next
	if( $1 ~ "アイテム" )	next

	print "_EVENT_DATA\t\t" "ev_fld_item_" $6 >> file1

	#fld_item_flag.h--------------------------------------------------------------
	#print "#define FV_BALL_" $6 " (FV_BALL_START+" $6 ")" >> file2
}

END{
	print "_EVENT_DATA\t\t" "ev_fld_item2" >> file1
	print "_EVENT_DATA_END\t\t\t\t//終了" >> file1
	print "" >> file1
}

