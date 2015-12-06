/* 
 * Puppet Manifest Generator Tool
 * 2014/3-6 K.H
 */
#include "pmgt.h"

static void
P_usage(
  void
){
  char *us[] = {
"【名前】",
"　pmgt - Puppet マニフェスト生成ツール(PMGT)  Ver.0.12.0",
"",
"【形式】",
"　pmgt [オプション] {モード}",
"",
"【説明】",
"　　PMGT パラメータファイルを読み込み、モードに対応した各種ファイルを生成",
"　します。",
"",
"　　PMGT の制御は PMGT 制御ファイルで行います。",
"",
"【モード】",
"　gen",
"　　　PMGT パラメータファイルを読み込み、Puppet マニフェストを生成します。",
"",
"　hosts",
"　　　パラレル SSH 用のホスト一覧ファイルを生成します。",
"",
"　consts",
"　　　Puppet エージェント実行時の定数切り替え用ファイルを生成します。",
"",
"【オプション】",
"　--config path",
"　　　　指定された PMGT 制御ファイル「config.tab」を読み込みます。",
"　　　　(省略時：/etc/puppet/PMGT/config/config.tab)",
"",
"　--env env",
"　　　　指定された環境識別子の「puppet.conf_{環境識別子}」と",
"　　　「site_{環境識別子}.pp」を生成します。",
"　　　※「--module」と同時に指定された場合はエラー終了します。",
"",
"　--module module",
"　　　　指定されたモジュールのみ生成します。",
"　　　※「--env」と同時に指定された場合はエラー終了します。",
"",
"【終了ステータス】",
"　0   正常",
"　>0  エラー",
"",
"【pmgt 使用許諾】",
"",
"　Copyright (C) 2014/3-2014/6 K.H",
"",
"1.本ソフトウェアはフリー・ソフトウェアです。個人使用、業務使用に関わらず",
"　自由に使用できます。また、再配布についても自由です。",
"",
"2.本ソフトウェアは無保証です。自己責任で使用してください。",
"",
"3.不具合のご連絡、改善要望などは、下記の URL を参照して下さい。",
"",
"　　http://www7b.biglobe.ne.jp/~dump_struct/",
"",
    NULL
  };
  int i;

  for (i = 0; us[i]; ++i)
    {
      puts(us[i]);
    }
}

int
main(
  int argc,
  char **argv
){
  status_t status;
  Option_t option;
  Config_t config;
  Base_t base;

  (void)setlocale(LC_ALL, "");
  PCMsg_setHeader("pmgt");

  --argc;
  ++argv;

  if (argc == 0)
    {
      P_usage();

      return ABNORMAL;
    }

  PCMsg("START");

  status = PO_get(&option, &argc, argv);
  if (status) { PCMsg("end - %d", (int)status); return status; }

  if (argc < 1)
    {
      PCMsg_err("no mode\n");

      return ABNORMAL;
    }

  status = PM_loadConfig(&option, &config);
  if (status) { PCMsg("end - %d", (int)status); return status; }

  status = PM_loadBase(&option, &config, &base);
  if (status) { PCMsg("end - %d", (int)status); return status; }

  if (strcmp(argv[1], "gen") == 0)
    {
      status = PM_gen(&option, &config, &base);
    }
  else if (strcmp(argv[1], "hosts") == 0)
    {
      status = PH_hosts(&option, &config, &base);
    }
  else if (strcmp(argv[1], "consts") == 0)
    {
      status = PN_consts(&option, &config, &base);
    }
  else if (strcmp(argv[1], "test") == 0)
    {
      status = PT_test(&option, &config, &base);
    }
  else
    {
      status = ABNORMAL;
      PCMsg_err("not support mode - %s", argv[1]);
    }

  PPP_free(config.config);
  PPP_free(config.brackets);

  PPP_free(base.envs);
  PPP_free(base.hosts);
  PPP_free(base.modules);
  PPP_free(base.consts);

  if (status) { PCMsg("end - %d", (int)status); return status; }

  PCMsg("END - 0");

  return NORMAL;
}
