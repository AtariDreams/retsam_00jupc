{
	name = $0 
	sub( "SEQ_", "", name )

	#"SE_"に対応
	sub( "_", "-", name )

	#"DP_"に対応
	sub( "_", "-", name )

	#"_"に対応
	sub( "_", "-", name )
	sub( "_", "-", name )
	sub( "_", "-", name )
	sub( "_", "-", name )

	print name
}
