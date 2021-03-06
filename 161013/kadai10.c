//@brief 設問９と同様、entropyを求める問題。
/*@detail この課題では、一文字目のアルファベット別にエントロピーを求める.
すなわち、あるアルファベット（例えばa）が最初に出た時、
その後にそれぞれのアルファベットが出現する確率(p[a][*],*にはaに続くアルファベットが入ります。）
を９で行ったようにP(i)*(-1)*log2(P(i))を計算し、最後に足し合わせれば、このアルファベットaに対するエントロピーが出る。*/

/*これ以前の設問と重複するコードについては省略しています。*/
/*設問８よりcount[i][j]ができている前提。*/

  double entropy[NALPHA];//entropyはdoubleないしfloatで定義してください。
  int sum;
  for( i = 0 ; i < NALPHA ; i++ ) {
    sum = 0;

    for( j = 0 ; j < NALPHA ; j++ ) {
      sum += count[i][j];
    }
    for( j = 0 ; j < NALPHA ; j++ ) { 
      prob[i][j] = /*prob[i][j]の求め方は設問８を参考にしてください*/;
      if (/*9と同様、probが0となる場合は排除するためのconditionが必要です*/) {
      entropy[i] += /*９と同様*/;
      }
    }
  }

  for( i = 0 ; i < NALPHA ; i++ ) {
    printf( "[%c]: ent = %e\n", 'a' + i, entropy[i]);
  }
  return 0;
}
