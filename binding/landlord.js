/*
The MIT License (MIT)

Copyright (c) 2014 Master.G

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

var LL = (function() {
		var Card = {
			RANK_3 : 0x01,
			RANK_4 : 0x02,
			RANK_5 : 0x03,
			RANK_6 : 0x04,
			RANK_7 : 0x05,
			RANK_8 : 0x06,
			RANK_9 : 0x07,
			RANK_T : 0x08,
			RANK_J : 0x09,
			RANK_Q : 0x0A,
			RANK_K : 0x0B,
			RANK_A : 0x0C,
			RANK_2 : 0x0D,
			RANK_r : 0x0E,
			RANK_R : 0x0F,

			RANK_BEG : this.RANK_3,
			RANK_END : this.RANK_R + 1,

			SUIT_CLUB : 0x10,
			SUIT_DIAMOND : 0x20,
			SUIT_HEART : 0x30,
			SUIT_SPADE : 0x40,

			SET_LENGTH : 54,

			getRank: function(card) {
				return card & 0x0F;
			},

			getSuit: function(card) {
				return card & 0xF0;
			},

			isRed: function(card) {
				return this.getSuit(card) === this.SUIT_CLUB || this.getSuit(card) === this.SUIT_HEART;
			},

			make: function(suit, rank) {
				return suit | rank;
			},
			toString: function(card) {
				var _rankToStringTable = {
					0x01 : '3', 0x02 : '4', 0x03 : '5', 0x04 : '6',
					0x05 : '7', 0x06 : '8', 0x07 : '9', 0x08 : 'T',
					0x09 : 'J', 0x0A : 'Q', 0x0B : 'K', 0x0C : 'A',
					0x0D : '2', 0x0E : 'r', 0x0F : 'R'
				};

				var _suitToStringTable = {
					0x10 : '♣', 0x20 : '♦', 0x40 : '♠', 0x30 : '♥'
				};

				var rank = this.getRank(card);
				var suit = this.getSuit(card);

				return _suitToStringTable[suit] + _rankToStringTable[rank];
			}
		};

	return {

		Card : Card,

		// CardArray
		CardArray: {
			_stringToRankTable : {
				'3' : 0x01, '4' : 0x02, '5' : 0x03, '6' : 0x04,
				'7' : 0x05, '8' : 0x06, '9' : 0x07, 'T' : 0x08,
				'J' : 0x09, 'Q' : 0x0A, 'K' : 0x0B, 'A' : 0x0C,
				'2' : 0x0D, 'r' : 0x0E, 'R' : 0x0F
			},

			_stringToSuitTable : {
				'♣' : 0x10, '♦' : 0x20, '♠' : 0x40, '♥' : 0x30,
				'c' : 0x10, 'd' : 0x20, 's' : 0x40, 'h' : 0x30
			},

			InitFromString : function(str) {
				var raw = str.split(' ');
				var cooked = [];
				for (var i in raw) {
					var comp = raw[i];
					if (comp && comp.length > 0) {
						var rank = this._stringToRankTable[comp.slice(-1)];
						var suit = this._stringToSuitTable[comp.slice(0, -1)];
						if (rank && suit) {
							cooked.push(suit | rank);
						}
					}
				}

				return cooked;
			},

			InitFromDeck : function() {
				var deck = [
					0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
				    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
				    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
				    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,
				    0x1E, 0x2F
			    ];

			    return deck;
			},

			Concat: function(a, b) {
				return a.concat(b);
			},

			Subtract: function(a, b) {
				var exist = {};
				var ta = a.slice(0);

				for (var i in b) {
					exist[b[i]] = true;
				}

				for (var i in ta) {
					if (exist[ta[i]])
						ta.splice(i, 1);
				}

				return ta;
			},

			IsIdentity: function(a, b) {
				if (a === b) {
					return true;
				}

				if (a.length !== b.length) {
					return false;
				}

				var ta = a.sort();
				var tb = b.sort();

				for (var i = 0; i < ta.length; i++) {
					if (ta[i] !== tb[i])
						return false;
				}

				return true;
			},

			IsContain: function(a, b) {
				if (a === b)
					return true;
				if (a.length === 0 || b.length === 0)
					return false;
				if (a.length < b.length)
					return false;

				var ta = a.slice(0).sort();
				var tb = b.slice(0).sort();

				for (var i in tb) {
					for (var j in ta) {
						if (tb[i] === ta[j]) {
							tb.splice(i, 1);
							continue;
						}
					}
				}

				return tb.length === 0;
			},

			// PushBack = push
			// PushFront = unshift
			// PopFront = shift
			// PopBack = pop
			// Insert = splice(i, 0, element) arr[i] = element
			// Remove = splice(i, 1) arr[i] gone

			RemoveCard: function(a, card) {
				var ta = [];
				for (var i in a) {
					if (a[i] !== card) {
						ta.push(a[i]);
					}
				}

				return ta;
			},

			Sort: function(arr, comp) {
				var standardComparator = function(a, b) {
					var ra = ((a & 0xF0) >> 4) | ((a & 0x0F) << 4);
					var rb = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);
					return rb - ra;
				}

				arr.sort(standardComparator || comp);
			},

			// Reverse = reverse

			ToString: function(arr) {
				var str = "";
				for (var i in arr) {
					str += Card.CardToString(arr[i]);
					str += " ";
				}

				return str;
			}
		}
	};

})();
