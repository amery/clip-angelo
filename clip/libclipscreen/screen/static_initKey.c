static void
initKey(ScreenData * dp)
{
   Keytab *kp;

   Keytab *map = dp->keytab;

   int i;

   char *s;

   memcpy(map, init_keytab, sizeof(init_keytab));
   map[0].str = dp->meta1_key;
   map[1].str = dp->meta2_key;
   map[2].str = dp->national_key;

   map[3].str = get_key(dp, NO_kcub1);	/* kl */
   map[4].str = get_key(dp, NO_kcuf1);	/* kr */
   map[5].str = get_key(dp, NO_kcuu1);	/* ku */
   map[6].str = get_key(dp, NO_kcud1);	/* kd */

   map[7].str = get_key(dp, NO_knp);	/* kN */
   map[8].str = get_key(dp, NO_kpp);	/* kP */
   s = get_key(dp, NO_khome);	/* kh */
   if (!s)
      s = get_key(dp, NO_kend);	/* @7 */
   map[9].str = s;
   map[10].str = get_key(dp, NO_kll);	/* kH */
   map[11].str = get_key(dp, NO_kich1);	/* kI */
   map[12].str = get_key(dp, NO_kbs);	/* kb */
   map[13].str = get_key(dp, NO_kdch1);	/* kD */

   map[14].str = get_key(dp, NO_kf1);	/* k1 */
   map[15].str = get_key(dp, NO_kf2);	/* k2 */
   map[16].str = get_key(dp, NO_kf3);	/* k3 */
   map[17].str = get_key(dp, NO_kf4);	/* k4 */
   map[18].str = get_key(dp, NO_kf5);	/* k5 */
   map[19].str = get_key(dp, NO_kf6);	/* k6 */
   map[20].str = get_key(dp, NO_kf7);	/* k7 */
   map[21].str = get_key(dp, NO_kf8);	/* k8 */
   map[22].str = get_key(dp, NO_kf9);	/* k9 */
   map[23].str = get_key(dp, NO_kf0);	/* k0 */

   map[24].str = get_key(dp, NO_kf11);	/* k11 */
   map[25].str = get_key(dp, NO_kf12);	/* k12 */
   map[26].str = get_key(dp, NO_kf13);	/* k13 */
   map[27].str = get_key(dp, NO_kf14);	/* k14 */
   map[28].str = get_key(dp, NO_kf15);	/* k15 */
   map[29].str = get_key(dp, NO_kf16);	/* k16 */
   map[30].str = get_key(dp, NO_kf17);	/* k17 */
   map[31].str = get_key(dp, NO_kf18);	/* k18 */
   map[32].str = get_key(dp, NO_kf19);	/* k19 */
   map[33].str = get_key(dp, NO_kf20);	/* k20 */

   qsort(map, KEYTAB_SIZE, sizeof(Keytab), (int (*)(const void *, const void *)) compkeys);

   dp->key_count = 0;
   for (kp = map, i = 0; kp->val && i < KEYTAB_SIZE; kp++, i++)
   {
      dp->key_count++;
      if (kp->str)
      {
	 kp->strlen = strlen(kp->str);
	 if (kp->str[0])
	 {
	    if (kp->str[0] < dp->lomatch)
	       dp->lomatch = kp->str[0];
	    if (kp->str[0] > dp->himatch)
	       dp->himatch = kp->str[0];
	 }
      }
      else
	 kp->strlen = 0;
   }

}
