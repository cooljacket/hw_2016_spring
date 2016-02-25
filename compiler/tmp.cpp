if (isdigit(c) || isalpha(c)) {
			// type maybe: {OP, BD}
			if (mode != 2 && idx > 0) {
				
				int type = OP;
				if (index[int(BD)].find(now) != index[int(BD)].end())
					type = BD;

				program.push_back(make_pair(now, type+1));
				idx = 0;

				printf("now=%s\ttype=%d\tmode=%d\n", now.data(), type, mode);
			}
			mode = 2;
		} else if (c != '#') {
			// type maybe: {KW, ID, UINT}
			if (mode != 1 && idx > 0) {
				int type = KW;


				if (isBD(now)) {
					
				} else {
					if (!iskw(now)) {
						if (isuint(now)) {
							type = UINT;
							if (index[int(UINT)].find(now) == index[int(UINT)].end())
								index[int(UINT)][now] = index[int(UINT)].size();
						} else if (isIdentifier(now)) {
							type = ID;
							if (index[int(ID)].find(now) == index[int(ID)].end())
								index[int(ID)][now] = index[int(ID)].size();
						}
					}
				}
				

				program.push_back(make_pair(now, type));
				idx = 0;

				printf("now=%s\ttype=%d\tmode=%d\n", now.data(), type+1, mode);
			}
			mode = 1;
		}