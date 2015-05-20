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

	// ----------------------------------------------------------------
	// CardArray
	// ----------------------------------------------------------------

	var CardArray = function(p) {
		this.cards = [];

		this._stringToRankTable = {
			'3' : 0x01, '4' : 0x02, '5' : 0x03, '6' : 0x04,
			'7' : 0x05, '8' : 0x06, '9' : 0x07, 'T' : 0x08,
			'J' : 0x09, 'Q' : 0x0A, 'K' : 0x0B, 'A' : 0x0C,
			'2' : 0x0D, 'r' : 0x0E, 'R' : 0x0F
		};

		this._stringToSuitTable = {
			'♣' : 0x10, '♦' : 0x20, '♠' : 0x40, '♥' : 0x30,
			'c' : 0x10, 'd' : 0x20, 's' : 0x40, 'h' : 0x30
		};

		if (p && p.constructor === Array) {
			this.cards = p.slice(0);
		}
		else if (p && p.constructor === this.constructor) {
			this.cards = p.cards.slice(0);
		}
		else if (p && p.constructor === Array) {
			this.cards = p.slice(0);
		}
		else if (typeof(p) === "string") {
			this.initFromString(p);
		}
	};

	CardArray.prototype = {

		_getArray: function(p) {
			var ret = [];
			if (p.constructor === Array) {
				ret = p.slice(0);
			}
			else if (p.constructor === this.constructor) {
				ret = p.cards.slice(0);
			}

			return ret;
		},

		initFromString: function(str) {
			var raw = str.split(' ');
			this.cards = [];
			for (var i in raw) {
				var comp = raw[i];
				if (comp && comp.length > 0) {
					var rank = this._stringToRankTable[comp.slice(-1)];
					var suit = this._stringToSuitTable[comp.slice(0, -1)];
					if (rank && suit) {
						this.cards.push(suit | rank);
					}
				}
			}
		},

		initFromDeck: function() {
			this.cards = [
				0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
			    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
			    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
			    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,
			    0x1E, 0x2F
		    ];
		},

		concat: function(rhs) {
			var rcards = this._getArray(rhs);
			this.cards = this.cards.concat(rcards);
		},

		subtract: function(rhs) {
			var rcards = this._getArray(rhs);

			while (rhs.length > 0) {
				var f = rhs.shift();
				for (var i in this.cards) {
					if (this.cards[i] === f) {
						this.cards.splice(i, 1);
						break;
					}
				}
			}
		},

		size: function() {
			return this.cards.length;
		},

		isEqual: function(rhs) {
			if (rhs === this) {
				return true;
			}

			var rcards = this._getArray(rhs);

			if (rcards.length !== this.cards.length) {
				return false;
			}

			var lcards = this.cards.slice(0);

			rcards.sort();
			lcards.sort();
			for (var i = 0; i < lcards.length; i++) {
				if (lcards[i] !== rcards[i]) {
					return false;
				}
			}

			return true;
		},

		contain: function(rhs) {
			if (rhs === this) {
				return true;
			}

			var rcards = this._getArray(rhs);

			var lcards = this.cards.slice(0);

			if (lcards.length === 0 || rcards.length === 0) {
				return false;
			}

			if (lcards.length < rcards.length) {
				return false;
			}

			rcards.sort();
			lcards.sort();

			for (var i in rcards) {
				for (var j in lcards) {
					if (rcards[i] === lcards[j]) {
						rcards.splice(i, 1);
						continue;
					}
				}
			}

			return rcards.length === 0;
		},

		push_back: function(card) {
			this.cards.push(card);
		},

		push_front: function(card) {
			this.cards.unshift(card);
		},

		pop_front: function() {
			return this.cards.shift();
		},

		pop_back: function() {
			return this.cards.pop();
		},

		drop_front: function(count) {
			if (this.cards.length <= count) {
				count = this.cards.length;
				this.cards = [];
			}
			else {
				this.cards.splice(0, count);
			}

			return count;
		},

		drop_back: function(count) {
			if (this.cards.length <= count) {
				count = this.cards.length;
				this.cards = [];
			}
			else {
				this.cards.splice(-count);
			}

			return count;
		},

		pushBackCards: function(cards, where, count) {
			var tmp = this._getArray(cards);

			this.cards = this.cards.concat(tmp.splice(where, count));
		},

		copyRank: function(cards, rank) {
			var tmp = this._getArray(cards);
			for (var i = 0; i < tmp.length; i++) {
				if (Card.getRank(tmp[i]) === rank) {
					this.push_back(tmp[i]);
				}
			}
		},

		removeRank: function(rank) {
			var tmp = [];
			for (var i = 0; i < this.cards.length; i++) {
				if (Card.getRank(this.cards[i]) !== rank) {
					tmp.push(this.cards[i]);
				}
			}

			this.cards = tmp.slice(0);
		},

		insert: function(i, card) {
			this.cards.splice(i, 0, card);
		},

		remove: function(i) {
			return this.cards.splice(i, 1)[0];
		},

		removeCard: function(card) {
			var tmp = [];
			for (var i in this.cards) {
				if (this.cards[i] !== card) {
					tmp.push(this.cards[i]);
				}
			}

			this.cards = tmp.splice(0);
		},

		sort: function(comp) {
			var standardComparator = function(a, b) {
				var ra = ((a & 0xF0) >> 4) | ((a & 0x0F) << 4);
				var rb = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);
				return rb - ra;
			}

			this.cards.sort(standardComparator || comp);
		},

		reverse: function() {
			this.cards.reverse();
		},

		clear: function() {
			this.cards = [];
		},

		clone: function() {
			return new CardArray(this.cards);
		},

		copy: function(rhs) {
			if (rhs.constructor === Array) {
				this.cards = rhs.slice(0);
			}
			else if (rhs.constructor === this.constructor) {
				this.cards = rhs.cards.slice(0);
			}
		},

		toArray: function() {
			return this.cards.slice(0);
		},

		toString: function() {
			var str = "";
			for (var i in this.cards) {
				str += Card.toString(this.cards[i]);
				str += " ";
			}

			return str;
		},

		get length() {
			return this.cards.length;
		}
	};

	CardArray.prototype.constructor = CardArray;

	// ----------------------------------------------------------------
	// Deck
	// ----------------------------------------------------------------

	var Deck = function() {
		this.cards = new CardArray();
		this.cards.initFromDeck();
		this.used = new CardArray();
	};

	Deck.prototype = {
		shuffle: function() {
			var cards = this.cards.cards;
			var i = cards.length;
			var j, tmp;

			while (--i > 0) {
				j = Math.floor(Math.random() * (i + 1));

				tmp = cards[j];
				cards[j] = cards[i];
				cards[i] = tmp;
			}
		},

		reset: function() {
			this.cards.initFromDeck();
			this.used.clear();
		},

		deal: function(num) {
			if (num) {
				return new CardArray(this.cards.cards.splice(-num));
			}
			else {
				return this.cards.pop_back();
			}
		},

		recycle: function(card) {
			if (card && typeof card === "number") {
				this.used.push_back(card);
			}
			else {
				this.used.concat(card);
			}
		}
	};

	Deck.prototype.constructor = Deck;

	// ----------------------------------------------------------------
	// Hand
	// ----------------------------------------------------------------

	var Hand = function() {
		this.type = 0;
		this.cards = new CardArray();
	};

	// compare result
	Hand.CMP_GREATER = 1;
	Hand.CMP_LESS 	 = -1;
	Hand.CMP_EQUAL	 = 0;

	// primal hands
	Hand.PRIMAL_NONE = 0x00;
	Hand.PRIMAL_NUKE = 0x06;
	Hand.PRIMAL_BOMB = 0x05;
	Hand.PRIMAL_FOUR = 0x04;
	Hand.PRIMAL_TRIO = 0x03;
	Hand.PRIMAL_PAIR = 0x02;
	Hand.PRIMAL_SOLO = 0x01;

	// kicker hands
	Hand.KICKER_NONE = 0x00;
	Hand.KICKER_SOLO = 0x10;
	Hand.KICKER_PAIR = 0x20;
	Hand.KICKER_DUAL_SOLO = 0x30;
	Hand.KICKER_DUAL_PAIR = 0x40;

	// chain
	Hand.CHAIN_NONE = 0x00;
	Hand.CHAINED = 0x80;

	// constants
	Hand.MIN_LENGTH = 1;
	Hand.MAX_LENGTH = 56;
	Hand.SOLO_CHAIN_MIN_LENGTH = 5;
	Hand.PAIR_CHAIN_MIN_LENGTH = 6;
	Hand.TRIO_CHAIN_MIN_LENGTH = 6;
	Hand.FOUR_CHAIN_MIN_LENGTH = 8;

	// pattern
	Hand.PATTERN_LENGTH = 12;

	Hand.PATTERN_NONE =  0;   /* place holder */
	Hand.PATTERN_1 =     1;   /* 1, solo */
	Hand.PATTERN_2_1 =   2;   /* 2, pair */
	Hand.PATTERN_2_2 =   3;   /* 2, nuke */
	Hand.PATTERN_3 =     4;   /* 3, trio */
	Hand.PATTERN_4_1 =   5;   /* bomb */
	Hand.PATTERN_4_2 =   6;   /* trio solo */
	Hand.PATTERN_5_1 =   7;   /* solo chain */
	Hand.PATTERN_5_2 =   8;   /* trio pair */
	Hand.PATTERN_6_1 =   9;   /* solo chain */
	Hand.PATTERN_6_2 =   10;  /* pair chain */
	Hand.PATTERN_6_3 =   11;  /* trio chain */
	Hand.PATTERN_6_4 =   12;  /* four dual solo */
	Hand.PATTERN_8_1 =   13;  /* solo chain */
	Hand.PATTERN_8_2 =   14;  /* pair chain */
	Hand.PATTERN_8_3 =   15;  /* trio solo chain */
	Hand.PATTERN_8_4 =   16;  /* four dual pair */
	Hand.PATTERN_8_5 =   17;  /* four chain */
	Hand.PATTERN_9_1 =   18;  /* solo chain */
	Hand.PATTERN_9_2 =   19;  /* trio chain */
	Hand.PATTERN_10_1 =  20;  /* solo chain */
	Hand.PATTERN_10_2 =  21;  /* pair chain */
	Hand.PATTERN_10_3 =  22;  /* trio pair chain */
	Hand.PATTERN_12_1 =  23;  /* solo chain */
	Hand.PATTERN_12_2 =  24;  /* pair chain */
	Hand.PATTERN_12_3 =  25;  /* trio chain */
	Hand.PATTERN_12_4 =  26;  /* trio solo chain */
	Hand.PATTERN_12_5 =  27;  /* four chain */
	Hand.PATTERN_12_6 =  28;  /* four dual solo chain */
	Hand.PATTERN_16_1 =  29;  /* pair chain */
	Hand.PATTERN_16_2 =  30;  /* trio solo chain */
	Hand.PATTERN_16_3 =  31;  /* four chain */
	Hand.PATTERN_16_4 =  32;  /* four dual pair chain */
	Hand.PATTERN_18_1 =  33;  /* pair chain */
	Hand.PATTERN_18_2 =  34;  /* trio chain */
	Hand.PATTERN_18_3 =  35;  /* four dual solo chain */
	Hand.PATTERN_20_1 =  36;  /* pair chain */
	Hand.PATTERN_20_2 =  37;  /* trio solo chain */
	Hand.PATTERN_20_3 =  38;  /* four chain */
	Hand.PATTERN_END =   Hand.PATTERN_20_3;

	Hand.patterns = [
		[ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* place holder */
	    [ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 1, solo */
	    [ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 2, pair */
	    [ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 2, nuke */
	    [ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 3, trio */
	    [ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 4, bomb */
	    [ 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 4, trio solo */
	    [ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 ],     /* 5, solo chain */
	    [ 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 5, trio pair */
	    [ 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 ],     /* 6, solo chain */
	    [ 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 6, pair chain */
	    [ 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 6, trio chain */
	    [ 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 6, four dual solo */
	    [ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 ],     /* 8, solo chain */
	    [ 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 8, pair chain */
	    [ 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 8, trio solo chain */
	    [ 4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 8, four dual pair */
	    [ 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 8, four chain */
	    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 ],     /* 9, solo chain */
	    [ 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 9, trio chain */
	    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 ],     /* 10, solo chain */
	    [ 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0 ],     /* 10, pair chain */
	    [ 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 10, trio pair chain */
	    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ],     /* 12, solo chain */
	    [ 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 ],     /* 12, pair chain */
	    [ 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 12, trio chain */
	    [ 3, 3, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0 ],     /* 12, trio solo chain */
	    [ 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 12, four chain */
	    [ 4, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 ],     /* 12, four dual solo chain */
	    [ 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0 ],     /* 16, pair chain */
	    [ 3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0 ],     /* 16, trio solo chain */
	    [ 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 ],     /* 16, four chain */
	    [ 4, 4, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 ],     /* 16, four dual pair chain */
	    [ 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 ],     /* 18, pair chain */
	    [ 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0 ],     /* 18, trio chain */
	    [ 4, 4, 4, 1, 1, 1, 1, 1, 1, 0, 0, 0 ],     /* 18, four dual solo chain */
	    [ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0 ],     /* 20, pair chain */
	    [ 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 0, 0 ]      /* 20, trio solo chain */
	];

	Hand.SortCount = function(count) {
		count.sort();
		count.reverse();
	};

	Hand.CountRank = function(cards, sort) {
		var count = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ];
		var sorted = [];

		var c = [];
		if (cards.constructor === Array) {
			c = cards.slice(0);
		} else if (cards.constructor === CardArray) {
			c = cards.cards.slice(0);
		}

		for (var i = 0; i < c.length; i++) {
			var r = Card.getRank(c[i]);
			count[r]++;
		}

		if (sort) {
			sorted = count.slice(0);
			sorted.sort();
			sorted.reverse();
		}

		return [count, sorted];
	};

	// check if a sorted count array matches specific pattern
	Hand.PatternMatch = function(sorted, pattern) {
		var p = Hand.patterns[pattern];
		if (!p)
			return false;

		for (var i = 0; i < p.length; i++) {
			if (sorted[i] !== p[i])
				return false;
		}

		return true;
	};

	// check if pattern as 334455 666777 etc
	// | 666 | 777 | 888 | 999 |
	// | 123 |					 duplicate: 3
	// |  1     2     3     4  | expectLength: 4
	Hand.CheckChain = function(count, duplicate, expectLength) {
		var marker = 0;
		var length = 0;

		// joker and 2 can't chain up
		for (var i = Card.RANK_3; i < Card.RANK_2; i++) {
			// found first match
			if (count[i] === duplicate && marker === 0) {
				marker = i;
				continue;
			}

			// matches ended
			if ((count[i] !== duplicate && marker !== 0)) {
				length = i - marker;
				break;
			}
		}

		return (length === expectLength);
	};

	Hand.prototype = {
		parse: function(cards) {
			var hand_parse_1 = function(hand, arr, count, sorted) {
				// solo
				hand.cards.copy(arr);
				hand.format(Hand.PRIMAL_SOLO, Hand.KICKER_NONE, Hand.CHAIN_NONE);
			}

			var hand_parse_2 = function(hand, arr, count, sorted) {
				// pair or nuke
				if (Card.getRank(arr.cards[0]) === Card.getRank(arr.cards[1])) {
					hand.cards.copy(arr);
					hand.format(Hand.PRIMAL_PAIR, Hand.KICKER_NONE, Hand.CHAIN_NONE);
				}
				else if (Card.getRank(arr.cards[0]) === Card.RANK_R && Card.getRank(arr.cards[1]) === Card.RANK_r) {
					hand.cards.copy(arr);
					hand.format(Hand.PRIMAL_NUKE, Hand.KICKER_NONE, Hand.CHAIN_NONE);
				}
			}

			var hand_parse_3 = function(hand, arr, count, sorted) {
				// trio
				var rank1 = Card.getRank(arr.cards[0]);
				var rank2 = Card.getRank(arr.cards[1]);
				var rank3 = Card.getRank(arr.cards[2]);
				if (rank1 === rank2 && rank2 === rank3) {
					hand.cards.copy(arr);
					hand.type = Hand.PRIMAL_TRIO;
				}
			}

			var hand_parse_4 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_4_1)) {
					// bomb, 4
					hand.cards.copy(arr);
					hand.format(Hand.PRIMAL_BOMB, Hand.KICKER_NONE, Hand.CHAIN_NONE);
				}
				else if (Hand.PatternMatch(sorted, Hand.PATTERN_4_2)) {
					// trio solo, 3-1
					this.distribute(arr, count, 3, 1, 4);
					hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_SOLO, Hand.CHAIN_NONE);
				}
			}

			var hand_parse_5 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_5_2)) {
					// trio pair, 3-2
					this.distribute(arr, count, 3, 2, 5);
					hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_PAIR, Hand.CHAIN_NONE);
				}
			}

			var hand_parse_6 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_6_4)) {
					// four dual solo, 4-1-1
					this.distribute(arr, count, 4, 1, 6);
					hand.format(Hand.PRIMAL_FOUR, Hand.KICKER_DUAL_SOLO, Hand.CHAIN_NONE);
				}
			}

			var hand_parse_8 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_8_3)) {
					// trio solo chain
					if (Hand.CheckChain(count, 3, 2)) {
						this.distribute(arr, count, 3, 1, 8);
						hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_SOLO, Hand.CHAINED);
					}
				}
				else if (Hand.PatternMatch(sorted, Hand.PATTERN_8_4)) {
					// four dual pair, no joker
					this.distribute(arr, count, 4, 2, 8);
					hand.format(Hand.PRIMAL_FOUR, Hand.KICKER_DUAL_PAIR, Hand.CHAIN_NONE);
				}
			}

			var hand_parse_10 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_10_3)) {
					// trio pair chain, no joker
					if (Hand.CheckChain(count, 3, 2)) {
						hand.distribute(arr, count, 3, 2, 10);
						hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_PAIR, Hand.CHAINED);
					}
				}
			}

			var hand_parse_12 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_12_4)) {
					// trio solo chain
					if (Hand.CheckChain(count, 3, 3)) {
						hand.distribute(arr, count, 3, 1, 12);
						hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_PAIR, Hand.CHAINED);
					}
				}
				else if (Hand.PatternMatch(sorted, Hand.PATTERN_12_6)) {
					// four dual solo chain
					if (Hand.CheckChain(count, 4, 2)) {
						Hand.distribute(arr, count, 4, 1, 12);
						hand.format(Hand.PRIMAL_FOUR, Hand.KICKER_DUAL_SOLO, Hand.CHAINED);
					}
				}
			}

			var hand_parse_16 = function(hand, arr, count, sorted) {
				// trio solo chain
				if (Hand.PatternMatch(sorted, Hand.PATTERN_16_2)) {
					if (Hand.CheckChain(count, 3, 4)) {
						hand.distribute(arr, count, 3, 1, 16);
						hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_SOLO, Hand.CHAINED);
					}
				}
				else if (Hand.PatternMatch(sorted, Hand.PATTERN_16_4)) {
					// four dual pair chain
					if (Hand.CheckChain(count, 4, 2)) {
						hand.distribute(arr, count, 4, 2, 16);
						hand.format(Hand.PRIMAL_FOUR, Hand.KICKER_DUAL_PAIR, Hand.CHAINED);
					}
				}
			}

			var hand_parse_18 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_18_3)) {
					// four dual solo chain
					if (Hand.CheckChain(count, 4, 3)) {
						hand.distribute(arr, count, 4, 1, 18);
						hand.format(Hand.PRIMAL_FOUR, Hand.KICKER_DUAL_SOLO, Hand.CHAINED);
					}
				}
			}

			var hand_parse_20 = function(hand, arr, count, sorted) {
				if (Hand.PatternMatch(sorted, Hand.PATTERN_20_2)) {
					if (Hand.CheckChain(count, 3, 5)) {
						hand.distribute(arr, count, 3, 1, 20);
						hand.format(Hand.PRIMAL_TRIO, Hand.KICKER_SOLO, Hand.CHAINED);
					}
				}
			}

			var hand_parse_default = function(hand, arr, count, sorted) {
				hand.type = 0;
			}

			// parser function table
			var parser = [];
			parser[Hand.MIN_LENGTH + 0]     = hand_parse_1;
		    parser[Hand.MIN_LENGTH + 1]     = hand_parse_2;
		    parser[Hand.MIN_LENGTH + 2]     = hand_parse_3;
		    parser[Hand.MIN_LENGTH + 3]     = hand_parse_4;
		    parser[Hand.MIN_LENGTH + 4]     = hand_parse_5;
		    parser[Hand.MIN_LENGTH + 5]     = hand_parse_6;
		    parser[Hand.MIN_LENGTH + 6]     = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 7]     = hand_parse_8;
		    parser[Hand.MIN_LENGTH + 8]     = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 9]     = hand_parse_10;
		    parser[Hand.MIN_LENGTH + 10]    = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 11]    = hand_parse_12;
		    parser[Hand.MIN_LENGTH + 12]    = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 13]    = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 14]    = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 15]    = hand_parse_16;
		    parser[Hand.MIN_LENGTH + 16]    = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 17]    = hand_parse_18;
		    parser[Hand.MIN_LENGTH + 18]    = hand_parse_default;
		    parser[Hand.MIN_LENGTH + 19]    = hand_parse_20;

		    // sort cards
		    cards.sort();

		    // count ranks
		    var ret = Hand.CountRank(cards, true);
		    var count = ret[0];
		    var sorted = ret[1];
		    var length = cards.length;

		    // clear hand
		    this.clear();

		    // validate length
		    if (length < Hand.MIN_LENGTH || length > Hand.MAX_LENGTH) {
		    	return;
		    }
		    // solo chain
		    else if (length >= Hand.SOLO_CHAIN_MIN_LENGTH && Hand.CheckChain(count, 1, length)) {
		    	this.cards.copy(cards);
		    	this.format(Hand.PRIMAL_SOLO, Hand.KICKER_NONE, Hand.CHAINED);
		    }
		    // pair chain
		    else if (length >= Hand.PAIR_CHAIN_MIN_LENGTH && length % 2 === 0 && Hand.CheckChain(count, 2, length / 2)) {
		    	this.cards.copy(cards);
		    	this.format(Hand.PRIMAL_PAIR, Hand.KICKER_NONE, Hand.CHAINED);
		    }
		    // trio chain
		    else if (length >= Hand.TRIO_CHAIN_MIN_LENGTH && length % 3 === 0 && Hand.CheckChain(count, 3, length / 3)) {
		    	this.cards.copy(cards);
		    	this.format(Hand.PRIMAL_TRIO, Hand.KICKER_NONE, Hand.CHAINED);
		    }
		    // four chain
		    else if (length >= Hand.FOUR_CHAIN_MIN_LENGTH && length % 4 === 0 && Hand.CheckChain(count, 4, length / 4)) {
		    	this.cards.copy(cards);
		    	this.format(Hand.PRIMAL_FOUR, Hand.KICKER_NONE, Hand.CHAINED);
		    }
		    // other types
		    else {
		    	parser[length](this, cards, count, sorted);
		    }
		},

		// distribute cards
		// for example, distribute(xxx, 88666644, 422, 4, 2, 8)
		// result in 66668844
		distribute: function(arr, count, d1, d2, length) {
			var temp = new CardArray();
			var cards = arr.cards;

			this.clear();

			for (var i = 0; i < cards.length; i++) {
				var card = cards[i];
				var num = count[Card.getRank(card)];

				if (num === d1) {
					this.cards.push_back(card);
				}
				else if (num === d2) {
					temp.push_back(card);
				}

				if (this.cards.length + temp.length >= length) {
					this.cards.concat(temp);
					break;
				}
			}
		},

		clear: function() {
			this.cards.clear();
			this.type = 0;
		},

		copy: function(rhs) {
			this.cards = rhs.cards.clone();
			this.type = rhs.type;
		},

		clone: function() {
			var hand = new Hand();
			hand.cards = this.cards.clone();
			hand.type = this.type;
			return hand;
		},

		compare: function(rhs) {
			var compare_bomb = function(ha, hb) {
				var ret = undefined;

				// same type same cards, equal
				if ((ha.type === hb.type) && (ha.cards.cards[0] === hb.cards.cards[0])) {
					ret = Hand.CMP_EQUAL;
				}
				// both are bombs, compare by card rank
				else if (ha.type === Hand.PRIMAL_BOMB && hb.type === Hand.PRIMAL_BOMB) {
					ret = Card.getRank(ha.cards.cards[0]) > Card.getRank(hb.cards.cards[0]) ? Hand.CMP_GREATER : Hand.CMP_LESS;
				}
				else {
					ret = ha.getPrimal() > hb.getPrimal() ? Hand.CMP_GREATER : Hand.CMP_LESS;
				}

				return ret;
			}

			var ret = undefined;

			// different hand types
			// check for bomb and nuke
			if (this.type !== rhs.type) {
				if (this.type !== Hand.PRIMAL_NUKE &&
					this.type !== Hand.PRIMAL_BOMB &&
					rhs.type !== Hand.PRIMAL_NUKE &&
					rhs.type !== Hand.PRIMAL_BOMB)
					return ret;
				else
					ret = compare_bomb(this, rhs);
			}
			// same hand types and with no bombs
			else {
				// same hand types but different length
				if (this.cards.length !== rhs.cards.length) {
					return ret;
				}
				else {
					// same hand types and same length
					if (Card.getRank(this.cards.cards[0]) === Card.getRank(rhs.cards.cards[0])) {
						ret = Hand.CMP_EQUAL;
					}
					else {
						ret = Card.getRank(this.cards.cards[0]) > Card.getRank(rhs.cards.cards[0]) ? Hand.CMP_GREATER : Hand.CMP_LESS;
					}
				}
			}

			return ret;
		},

		getPrimal: function() {
			return (this.type & 0x0F);
		},

		getKicker: function() {
			return (this.type & 0x70);
		},

		getChain: function() {
			return (this.type & 0x80);
		},

		setPrimal: function(p) {
			if (p)
				this.type |= p;
			else
				this.type &= 0xF0;
		},

		setKicker: function(k) {
			if (k)
				this.type |= k;
			else
				this.type &= 0x8F;
		},

		setChain: function(c) {
			if (typeof c === 'number') {
				this.type |= c;
			} else if (typeof c === 'boolean') {
				if (c)
					this.type |= Hand.CHAINED;
				else
					this.type &= 0x7F;
			}
		},

		format: function(primal, kicker, chained) {
			this.type = primal | kicker | chained;
		},

		toString: function() {
			var str = "";
			var primalTable = {};
			primalTable[Hand.PRIMAL_NONE] = "none";
			primalTable[Hand.PRIMAL_SOLO] = "solo";
			primalTable[Hand.PRIMAL_PAIR] = "pair";
			primalTable[Hand.PRIMAL_TRIO] = "trio";
			primalTable[Hand.PRIMAL_FOUR] = "four";
			primalTable[Hand.PRIMAL_BOMB] = "bomb";
			primalTable[Hand.PRIMAL_NUKE] = "nuke";

			var kickerTable = {};
			kickerTable[Hand.KICKER_NONE] = "";
			kickerTable[Hand.KICKER_SOLO] = "solo";
			kickerTable[Hand.KICKER_PAIR] = "pair";
			kickerTable[Hand.KICKER_DUAL_SOLO] = "dual solo";
			kickerTable[Hand.KICKER_DUAL_PAIR] = "dual pair";

			var primal = primalTable[this.getPrimal()];
			var kicker = kickerTable[this.getKicker()];
			str += primal || "";
			str += kicker ? " " + kicker : "";

			if (this.getChain()) {
				str += " chain";
			}

			return str;
		}

	};

	Hand.prototype.constructor = Hand;

	//
	// next_comb(int comb[], int k, int n)
	// Generates the next combination of n elements as k after comb
	//
	// comb => the previous combination ( use (0, 1, 2, ..., k) for first)
	// k => the size of the subsets to generate
	// n => the size of the original set
	//
	// Returns: 1 if a valid combination was found
	// 0, otherwise
	//
	var next_comb = function(comb, k, n) {
		var i = k - 1;
		++comb[i];
		while ((i > 0) && (comb[i] >= n - k + 1 + i)) {
			--i;
			++comb[i];
		}

		if (comb[0] > n - k)		// Combination (n-k, n-k+1, ..., n) reached
			return false;			// No more combinations can be generated

		// comb now looks like (..., x, n, n, n, ..., n).
     	// Turn it into (..., x, x + 1, x + 2, ...)
		for (i = i + 1; i < k; ++i) {
			comb[i] = comb[i - 1] + 1;
		}

		return true;
	}

	// ----------------------------------------------------------------
	// Beat Searching Context
	// ----------------------------------------------------------------

	var BeatSearchContext = function(cards) {
		if (!cards) {
			this.count = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ];
			this.cards = new CardArray();
			this.rcards = new CardArray();
		}
		else {
			this.setup(cards);
		}
	};

	BeatSearchContext.prototype = {
		clear: function() {
			this.count = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ];
			this.cards.clear();
			this.rcards.clear();
		},

		setup: function(cards) {
			this.count = Hand.CountRank(cards)[0];
			this.cards = cards.clone();
			this.cards.sort();
			this.rcards = this.cards.clone();
			this.rcards.reverse();
		}
	};

	BeatSearchContext.prototype.constructor = BeatSearchContext;

	// ----------------------------------------------------------------
	// HandList
	// ----------------------------------------------------------------

	var HandList = function() {
		this.hands = [];
	};

	HandList.SearchBeat_Primal = function(bsc, tobeat, beat, primal) {
		var tobeattype = tobeat.type;
		var count = bsc.count;
		var temp = bsc.rcards;
		var rank = Card.getRank(tobeat.cards.cards[0]);

		// search for primal
		for (var i = 0; i < temp.length; i++) {
			var tr = Card.getRank(temp.cards[i]);
			if (tr > rank && count[tr] >= primal) {
				beat.clear();
				beat.type = tobeattype;
				beat.cards.pushBackCards(temp, i, primal);
				return true;
			}
		}

		return false;
	};

	HandList.SearchBeat_Bomb = function(bsc, tobeat, beat) {
		var cards = bsc.cards;
		var count = bsc.count;
		var canbeat = false;

		// can't beat nuke
		if (tobeat.type === Hand.PRIMAL_NUKE) {
			return false;
		}

		// search for a higher rank bomb
		if (tobeat.type === Hand.PRIMAL_BOMB) {
			canbeat = HandList.SearchBeat_Primal(bsc, tobeat, beat, 4);
		}
		else {
			// tobeat is not a nuke or bomb, search a bomb to beat it
			for (var i = 0; i < cards.length; i++) {
				if (count[Card.getRank(cards.cards[i])] === 4) {
					canbeat = true;
					beat.clear();
					beat.cards.copyRank(cards, Card.getRank(cards.cards[i]));
					break;
				}
			}
		}

		// search for nuke
		if (!canbeat) {
			if (count[Card.RANK_r] !== 0 && count[Card.RANK_R] !== 0) {
				canbeat = true;
				beat.clear();
				beat.format(Hand.PRIMAL_NUKE, Hand.KICKER_NONE, Hand.CHAIN_NONE);
				beat.cards.copyRank(cards, Card.RANK_R);
				beat.cards.copyRank(cards, Card.RANK_r);
			}
		}
		else {
			beat.format(Hand.PRIMAL_BOMB, Hand.KICKER_NONE, Hand.CHAIN_NONE);
		}

		return canbeat;
	};

	//
	// for a standard 54 card set, each rank has four cards
	// so it is impossible for two trio of the same rank at the same time
	//
	// a) player_1 SEARCH_BEAT player_2 : impossible for 333 vs 333
	//
	// BUT
	//
	// b) player_1 SEARCH_BEAT player_1_prev_beat : possible for 333 vs 333
	//
	HandList.SearchBeat_TrioKicker = function(bsc, tobeat, beat, kick) {
		var canbeat = false;
		var cantriobeat = false;
		var tobeattype = tobeat.type;
		var count = bsc.count;
		var temp = bsc.rcards.clone();
		var htrio = new Hand();
		var hkick = new Hand();
		var htriobeat = new Hand();
		var hkickbeat = new Hand();

		// copy hands, trio and kicker
		htrio.cards.pushBackCards(tobeat.cards, 0, 3);
		hkick.cards.pushBackCards(tobeat.cards, 3, kick);

		// same rank trio, case b
		if (temp.contain(htrio.cards)) {
			// keep trio beat
			htriobeat.cards.copy(htrio.cards);
			temp.removeRank(Card.getRank(htriobeat.cards.cards[0]));

			// search for a higher kicker
			// round 1: only search those count[rank] === kick
			for (var i = 0; i < temp.length; i++) {
				if (count[Card.getRank(temp.cards[i])] >= kick &&
					Card.getRank(temp.cards[i]) > Card.getRank(hkick.cards.cards[0])) {

					hkickbeat.cards.clear();
					hkickbeat.cards.pushBackCards(temp, i, kick);
					canbeat = true;
					break;
				}
			}

			// if kicker can't beat, restore trio
			if (!canbeat) {
				htriobeat.cards.clear();
				temp = bsc.rcards.clone();
			}
		}

		// same rank trio not found
		// OR
		// same rank trio found, but kicker can't beat
		if (!canbeat) {
			cantriobeat = HandList.SearchBeat_Primal(bsc, htrio, htriobeat, Hand.PRIMAL_TRIO);
			// trio beat found, search for kicker beat
			if (cantriobeat) {
				// remove trio from temp
				temp.removeRank(Card.getRank(htriobeat.cards.cards[0]));

				// search for a kicker
				for (var i = 0; i < temp.length; i++) {
					if (count[Card.getRank(temp.cards[i])] >= kick) {
						hkickbeat.cards.pushBackCards(temp, i, kick);
						canbeat = true;
						break;
					}
				}
			}
		}

		// beat
		if (canbeat) {
			beat.clear();
			beat.cards.concat(htriobeat.cards);
			beat.cards.concat(hkickbeat.cards);
			beat.type = tobeattype;
		}

		return canbeat;
	};

	HandList.SearchBeat_Chain = function(bsc, tobeat, beat, duplicate) {
		var canbeat = false;
		var chainlength = 0;
		var found = false;
		var tobeattype = tobeat.type;
		var footer = 0;
		var count = bsc.count;
		var cards = bsc.cards;
		var temp = new CardArray();

		chainlength = tobeat.cards.length / duplicate;
		footer = Card.getRank(tobeat.cards.cards[tobeat.cards.length - 1]);

		// search for beat chain in rank counts
		for (var i = footer + 1; i <= Card.RANK_2 - chainlength; i++) {
			found = true;

			for (var j = 0; j < chainlength; j++) {
				// check if chain breaks
				if (count[i + j] < duplicate) {
					found = false;
					break;
				}
			}

			if (found) {
				footer = i;		 	// beat footer rank
				var k = duplicate;	// how many cards needed for each rank

				for (var i = cards.length; i >= 0 && chainlength > 0; i--) {
					if (Card.getRank(cards.cards[i]) === footer) {
						temp.push_front(cards.cards[i]);
						k--;

						if (k === 0) {
							k = duplicate;
							chainlength--;
							footer++;
						}
					}
				}

				break;
			}
		}

		if (found) {
			beat.type = tobeattype;
			beat.cards.copy(temp);
			canbeat = true;
		}

		return canbeat;
	};

	HandList.SearchBeat_TrioKickerChain = function(bsc, tobeat, beat, kc) {
		var canbeat = false;
		var chainlength = 0;
		var cantriobeat = false;
		var tobeattype = tobeat.type;
		var count = bsc.count.slice(0);
		var kickcount = [];
		var combrankmap = [];
		var rankcombmap = [];
		var comb = [];
		var temp = bsc.rcards.clone();
		var htrio = new Hand();
		var hkick = new Hand();
		var htriobeat = new Hand();
		var hkickbeat = new Hand();

		chainlength = tobeat.cards.length / (Hand.PRIMAL_TRIO + kc);

		// copy tobeat cards
		htrio.cards.pushBackCards(tobeat.cards, 0, 3 * chainlength);
		hkick.cards.pushBackCards(tobeat.cards, 3 * chainlength, chainlength * kc);

		htrio.format(Hand.PRIMAL_TRIO, Hand.KICKER_NONE, Hand.CHAINED);

		// self beat, see HandList.SearchBeat_TrioKicker
		if (temp.contain(htrio.cards)) {
			var n = 0;	// combination total

			// remove trio from kickcount
			kickcount = count.slice(0);
			for (var i = 0; i < htrio.cards.length; i += 3) {
				kickcount[Card.getRank(htrio.cards.cards[i])] = 0;
			}

			// remove count < kc and calculate n
			for (var i = Card.RANK_3; i < Card.RANK_END; i++) {
				if (kickcount[i] < kc) {
					kickcount[i] = 0;
				}
				else {
					n++;
				}
			}

			// setup comb-rank and rank-comb map
			var j = 0;
			combrankmap = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1];
			rankcombmap = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1];
			for (var i = 0; i < hkick.cards.length; i += kc) {
				comb[j++] = rankcombmap[Card.getRank(hkick.cards.cards[i])];
			}

			// find next combination
			if (next_comb(comb, chainlength, n)) {
				// next combination found, copy kickers
				for (var i = 0; i < chainlength; i++) {
					var rank = combrankmap[comb[i]];
					for (var j = 0; j < temp.length; j++) {
						if (Card.getRank(temp.cards[j]) === rank) {
							hkickbeat.cards.pushBackCards(temp, j, kc);
							break;
						}
					}
				}

				canbeat = true;
				// copy trio to beat
				htriobeat.cards.concat(htrio.cards);
				hkickbeat.cards.sort();
			}
		}

		// can't find same rank trio chain, search for higher rank trio
		if (!canbeat) {
			// restore rank count
			count = bsc.count.slice(0);
			cantriobeat = HandList.SearchBeat_Chain(bsc, htrio, htriobeat, 3);

			// higher rank trio chain found, search for kickers
			if (cantriobeat) {
				// remove trio from temp
				for (var i = 0; i < htriobeat.cards.length; i += 3) {
					temp.removeRank(Card.getRank(htriobeat.cards.cards[i]));
					count[Card.getRank(htriobeat.cards.cards[0])] = 0;
				}

				for (var j = 0; j < chainlength; j++) {
					for (var i = 0; i < temp.length; i++) {
						if (count[Card.getRank(temp.cards[i])] >= kc) {
							hkickbeat.cards.pushBackCards(temp, i, kc);
							temp.removeRank(Card.getRank(temp.cards[i]));
							break;
						}
					}
				}

				if (hkickbeat.cards.length === kc * chainlength) {
					canbeat = true;
				}
			}
		}

		// final
		if (canbeat) {
			beat.clear();
			beat.cards.concat(htriobeat.cards);
			beat.cards.concat(hkickbeat.cards);
			beat.type = tobeattype;
		}

		return canbeat;
	};

	HandList._SearchBeat = function(cards, tobeat, beat) {
		var canbeat = false;
		var bsc = new BeatSearchContext(cards);

		// start search
		switch (tobeat.type) {
			case (Hand.PRIMAL_SOLO | Hand.KICKER_NONE | Hand.CHAIN_NONE):
	            canbeat = HandList.SearchBeat_Primal(bsc, tobeat, beat, Hand.PRIMAL_SOLO);
	            break;
		    case (Hand.PRIMAL_PAIR | Hand.KICKER_NONE | Hand.CHAIN_NONE):
		        canbeat = HandList.SearchBeat_Primal(bsc, tobeat, beat, Hand.PRIMAL_PAIR);
		        break;
		    case (Hand.PRIMAL_TRIO | Hand.KICKER_NONE | Hand.CHAIN_NONE):
		        canbeat = HandList.SearchBeat_Primal(bsc, tobeat, beat, Hand.PRIMAL_TRIO);
		        break;
		    case (Hand.PRIMAL_TRIO | Hand.KICKER_PAIR | Hand.CHAIN_NONE):
		        canbeat = HandList.SearchBeat_TrioKicker(bsc, tobeat, beat, Hand.PRIMAL_PAIR);
		        break;
		    case (Hand.PRIMAL_TRIO | Hand.KICKER_SOLO | Hand.CHAIN_NONE):
		        canbeat = HandList.SearchBeat_TrioKicker(bsc, tobeat, beat, Hand.PRIMAL_SOLO);
		        break;
		    case (Hand.PRIMAL_SOLO | Hand.KICKER_NONE | Hand.CHAIN):
		        canbeat = HandList.SearchBeat_Chain(bsc, tobeat, beat, Hand.PRIMAL_SOLO);
		        break;
		    case (Hand.PRIMAL_PAIR | Hand.KICKER_NONE | Hand.CHAIN):
		        canbeat = HandList.SearchBeat_Chain(bsc, tobeat, beat, Hand.PRIMAL_PAIR);
		        break;
		    case (Hand.PRIMAL_TRIO | Hand.KICKER_NONE | Hand.CHAIN):
		        canbeat = HandList.SearchBeat_Chain(bsc, tobeat, beat, Hand.PRIMAL_TRIO);
		        break;
		    case (Hand.PRIMAL_FOUR | Hand.KICKER_NONE | Hand.CHAIN):
		        canbeat = HandList.SearchBeat_Chain(bsc, tobeat, beat, Hand.PRIMAL_FOUR);
		        break;
		    case (Hand.PRIMAL_TRIO | Hand.KICKER_PAIR | Hand.CHAIN):
		        canbeat = HandList.SearchBeat_TrioKickerChain(bsc, tobeat, beat, Hand.PRIMAL_PAIR);
		        break;
		    case (Hand.PRIMAL_TRIO | Hand.KICKER_SOLO | Hand.CHAIN):
		        canbeat = HandList.SearchBeat_TrioKickerChain(bsc, tobeat, beat, Hand.PRIMAL_SOLO);
		        break;
		    default:
		        break;
		}

		// search for bomb/nuke
		if (!canbeat) {
			canbeat = HandList.SearchBeat_Bomb(bsc, tobeat, beat);
		}

		return canbeat;
	};

	//
	// search for beat, result will be store in beat
	// 1. if [beat.type] !== 0, then search [new beat] > [beat]
	// 2. search [beat] > [tobeat], then store in [beat]
	//
	HandList.SearchBeat = function(cards, tobeat, beat) {
		// already in search loop, continue
		if (beat.type !== 0) {
			return HandList._SearchBeat(cards, beat, beat);
		}
		else {
			return HandList._SearchBeat(cards, tobeat, beat);
		}
	};

	// ----------------------------------------------------------------
	// Hand Analyzer
	// ----------------------------------------------------------------

	//
	// extract hands like 34567 / 334455 / 333444555 etc
	// array is a processed card array holds count[rank] === duplicate
	//
	HandList.ExtractConsecutive = function(hl, arr, duplicate) {
		var hand = new Hand();
		var primal = [0, Hand.PRIMAL_SOLO, Hand.PRIMAL_PAIR, Hand.PRIMAL_TRIO];
		var chainlen = [0, Hand.SOLO_CHAIN_MIN_LENGTH, Hand.PAIR_CHAIN_MIN_LENGTH, Hand.TRIO_CHAIN_MIN_LENGTH];

		if (duplicate < 1 || duplicate > 3 || arr.length === 0)
			return;

		var cardnum = arr.length / duplicate;
		var lastrank = Card.getRank(arr.cards[0]);
		var i = duplicate;

		while (cardnum--) {
			if ((lastrank - 1) !== Card.getRank(arr.cards[i])) {
				// chain break
				if (i >= chainlen[duplicate]) {
					// chain
					hand.clear();
					hand.format(primal[duplicate], Hand.KICKER_NONE, Hand.CHAINED);
					for (var j = 0; j < i; j++) {
						hand.cards.push_back(arr.pop_front());
					}

					hl.push_front(hand);
				}
				else {
					// not a chain
					for (var j = 0; j < i / duplicate; j++) {
						hand.clear();
						hand.format(primal[duplicate], Hand.KICKER_NONE, Hand.CHAIN_NONE);
						for (var k = 0; k < duplicate; k++) {
							hand.cards.push_back(arr.pop_front());
						}

						hl.push_front(hand);
					}
				}

				if (arr.length === 0) {
					break;
				}

				lastrank = Card.getRank(arr.cards[0]);
				i = duplicate;
			}
			else {
				// chain intact
				lastrank = Card.getRank(arr.cards[i]);
				i += duplicate;
			}
		}

		var k = i - duplicate;		// step back
		// all chain up
		if (k !== 0 && k === arr.length) {
			// can chain up
			if (k >= chainlen[duplicate]) {
				hand.clear();
				hand.format(primal[duplicate], Hand.KICKER_NONE, Hand.CHAINED);
				for (var j = 0; j < i - duplicate; j++) {
					hand.cards.push_back(arr.pop_front());
				}

				hl.push_front(hand);
			}
			else {
				for (var j = 0; j < k/duplicate; j++) {
					hand.clear();
					hand.format(primal[duplicate], Hand.KICKER_NONE, Hand.CHAIN_NONE);
					for (var i = 0; i < duplicate; i++) {
						hand.cards.push_back(arr.pop_front());
					}

					hl.push_front(hand);
				}
			}
		}
	};

	HandList.ExtractNukeBombJokerAndTwo = function(hl, arr, count) {
		var hand = new Hand();

		// nuke
		if (count[Card.RANK_r] !== 0 && count[Card.RANK_R] !== 0) {
			hand.clear();
			hand.format(Hand.PRIMAL_NUKE, Hand.KICKER_NONE, Hand.CHAIN_NONE);
			hand.cards.copyRank(arr, Card.RANK_R);
			hand.cards.copyRank(arr, Card.RANK_r);

			hl.push_front(hand);

			count[Card.RANK_r] = 0;
			count[Card.RANK_R] = 0;

			arr.removeRank(Card.RANK_r);
			arr.removeRank(Card.RANK_R);
		}

		// bomb
		for (var i = Card.RANK_2; i >= Card.RANK_3; i--) {
			if (count[i] === 4) {
				hand.clear();
				hand.format(Hand.PRIMAL_BOMB, Hand.KICKER_NONE, Hand.CHAIN_NONE);
				hand.cards.copyRank(arr, i);

				hl.push_front(hand);

				count[i] = 0;
				arr.removeRank(i);
			}
		}

		// joker
		if (count[Card.RANK_r] !== 0 || count[Card.RANK_R] !== 0) {
			hand.clear();
			hand.cards.copyRank(arr, count[Card.RANK_r] !== 0 ? Card.RANK_r : Card.RANK_R);
			hand.format(Hand.PRIMAL_SOLO, Hand.KICKER_NONE, Hand.CHAIN_NONE);

			hl.push_front(hand);
			count[Card.RANK_r] = 0;
			count[Card.RANK_R] = 0;
			arr.removeRank(Card.RANK_r);
			arr.removeRank(Card.RANK_R);
		}

		// 2
		if (count[Card.RANK_2] !== 0) {
			hand.clear();
			hand.cards.copyRank(arr, Card.RANK_2);

			var t = [0, Hand.PRIMAL_SOLO, Hand.PRIMAL_PAIR, Hand.PRIMAL_TRIO];
			hand.type = t[count[Card.RANK_2]];

			count[Card.RANK_2] = 0;
			arr.removeRank(Card.RANK_2);
			hl.push_front(hand);
		}
	};

	// ----------------------------------------------------------------
	// Advanced Hand Analyzer
	// ----------------------------------------------------------------

	HandList.SearchLongestConsecutive = function(bsc, hand, duplicate) {
		// context
		var primal = [0, Hand.PRIMAL_SOLO, Hand.PRIMAL_PAIR, Hand.PRIMAL_TRIO];
		var chainlen = [0, Hand.SOLO_CHAIN_MIN_LENGTH, Hand.PAIR_CHAIN_MIN_LENGTH, Hand.TRIO_CHAIN_MIN_LENGTH];
		var chain = new CardArray();
		var count = bsc.count;
		var cards = bsc.rcards;

		if (duplicate < 1 || duplicate > 3)
			return;

		// early break
		if (cards.length < chainlen[duplicate])
			return;

		// setup
		hand.clear();
		var rankstart = 0;
		var lastrank = Card.getRank(cards.cards[0]);

		//
		// i <= Card.RANK_2
		// but count[Card.RANK_2] must be 0
		// for 2 / bomb / nuke has been removed before calling this function
		//
		for (var i = Card.RANK_3; i <= Card.RANK_2; i++) {
			// find start of a possible chain
			if (rankstart === 0) {
				if (count[i] >= duplicate) {
					rankstart = i
				}

				continue;
			}

			// chain break
			if (count[i] < duplicate) {
				// chain break, extract chain and set new possible start
				if (((i - rankstart) * duplicate) >= chainlen[duplicate] && (i - rankstart) > chain.length) {
					// valid chain, store rank in CardArray
					chain.clear();
					for (var j = rankstart; j < i; j++) {
						chain.push_back(j);
					}
				}

				rankstart = 0;
			}
		}

		// convert rank array to card array
		if (chain.length > 0) {
			for (var i = chain.length - 1; i >= 0; i--) {
				lastrank = chain.cards[i];
				var k = duplicate;
				for (var j = 0; j < cards.length; j++) {
					if (Card.getRank(cards.cards[j]) === lastrank) {
						hand.cards.push_back(cards.cards[j]);
						k--;

						if (k === 0)
							break;
					}
				}
			}

			hand.format(primal[duplicate], Hand.KICKER_NONE, Hand.CHAINED);
		}
	};

	HandList.HAND_SEARCH_TYPES = 3;

	//
	// pass a empty hand to start traverse
	// result stores in hand
	// return false when stop
	//
	HandList.TraverseHands = function(bsc, begin, hand) {
		var found = false;
		var i = begin[0];
		var primals = [1, 2, 3];
		// solo chain, pair chain, trio chain, trio, pair, solo
		var searchers = [HandList.SearchLongestConsecutive, HandList.SearchLongestConsecutive, HandList.SearchLongestConsecutive];

		if (bsc.cards.length === 0) {
			return false;
		}

		if (begin[0] >= HandList.HAND_SEARCH_TYPES)
			return false;

		// init search
		if (hand.type === 0) {
			while (i < HandList.HAND_SEARCH_TYPES && hand.type === 0) {
				searchers[i](bsc, hand, primals[i]);
				if (hand.type !== 0) {
					found = true;
					begin[0] = i;
					break;
				}
				else {
					i++;
					begin[0] = i;
				}
			}

			// if found === false, should PANIC
		}
		// continue search via beat
		else {
			found = HandList.SearchBeat(bsc.cards, hand, hand);
		}

		return found;
	};

	// extract all chains or primal hands in beat_search_context
	HandList.ExtractAllChains = function(bsc, hands) {
		var found = false;
		var lastsearch = [0];
		var workinghand = new Hand();
		var lasthand = new Hand();

		// init search
		found = HandList.TraverseHands(bsc, lastsearch, lasthand);

		while (found) {
			hands.push_front(lasthand);
			workinghand.copy(lasthand);
			while ((found = HandList.TraverseHands(bsc, lastsearch, workinghand)))
				hands.push_front(workinghand);

			// can't find any more hands, try to reduce chain length
			if (lasthand.type !== 0) {
				if (lasthand.type === (Hand.PRIMAL_SOLO | Hand.KICKER_NONE | Hand.CHAINED)) {
					if (lasthand.cards.length > Hand.SOLO_CHAIN_MIN_LENGTH) {
						lasthand.cards.drop_front(1);
						found = true;
					}
					else {
						lasthand.type = 0;
					}
				}
				else if (lasthand.type === (Hand.PRIMAL_PAIR | Hand.KICKER_NONE | Hand.CHAINED)) {
					if (lasthand.cards.length > Hand.PAIR_CHAIN_MIN_LENGTH) {
						lasthand.cards.drop_front(2);
						found = true;
					}
					else {
						lasthand.type = 0;
					}
				}
				else if (lasthand.type === (Hand.PRIMAL_TRIO | Hand.KICKER_NONE | Hand.CHAINED)) {
					if (lasthand.cards.length > Hand.TRIO_CHAIN_MIN_LENGTH) {
						lasthand.cards.drop_front(3);
						found = true;
					}
					else {
						lasthand.type = 0;
					}
				}

				// still can't found, loop through hand type for more
				if (!found) {
					lastsearch[0]++;
					lasthand.clear();
					found = HandList.TraverseHands(bsc, lastsearch, lasthand);
				}
			}
		}
	};

	HandList.prototype = {
		clear: function() {
			this.hands = [];
		},

		push_back: function(hand) {
			this.hands.push(hand.clone());
		},

		push_front: function(hand) {
			this.hands.unshift(hand.clone());
		},

		pop_front: function() {
			return this.hands.shift();
		},

		pop_back: function() {
			return this.hands.pop();
		},

		remove: function(hand) {
			for (var i in this.hands) {
				if (this.hands[i] === hand) {
					return this.hands.splice(i, 1)[0];
				}
			}

			return undefined;
		},

		concat: function(hl) {
			var l = [];
			if (hl.constructor === Array) {
				l = hl;
			}
			else if (hl.constructor === this.constructor) {
				l = hl.hands;
			}

			this.hands = this.hands.concat(l);
		},

		find: function(type) {
			for (var i in this.hands) {
				if (this.hands[i].type === type) {
					return this.hands[i];
				}
			}

			return undefined;
		},

		size: function() {
			return this.hands.length;
		},

		searchBeatList: function(cards, tobeat) {
			this.hands = [];
			var htobeat = new Hand();
			var beat = new Hand();
			var canbeat = false;

			beat.clear();
			htobeat.copy(tobeat);

			do {
				canbeat = HandList.SearchBeat(cards, htobeat, beat);
				if (canbeat) {
					htobeat.copy(beat);
					this.push_front(beat);
				}

			} while (canbeat);
		},

		standardAnalyze: function(cards) {
			this.hands = [];

			var array = new CardArray(cards);
			var arrsolo = new CardArray();
			var arrpair = new CardArray();
			var arrtrio = new CardArray();
			var arrkicks = [arrsolo, arrpair, arrtrio];

			array.sort();
			var count = Hand.CountRank(array)[0];

			// nuke, bomb, joker and 2
			HandList.ExtractNukeBombJokerAndTwo(this, array, count);

			// chains
			for (var i = 0; i < array.length; ) {
				var c = count[Card.getRank(array.cards[i])];
				if (c !== 0) {
					arrkicks[c - 1].pushBackCards(array, i, c);
					i += c;
				}
				else {
					i++;
				}
			}

			// chain
			HandList.ExtractConsecutive(this, arrtrio, 3);
			HandList.ExtractConsecutive(this, arrpair, 2);
			HandList.ExtractConsecutive(this, arrsolo, 1);
		},

		advancedAnalyze: function(cards) {

		},

		bestBeat: function(cards, tobeat, beat, evalfunc) {

		},

		debugPrint: function() {

		},

		get length() {
			return this.hands.length;
		}
	};

	HandList.prototype.constructor = HandList;

	// ----------------------------------------------------------------

	return {

		Card: Card,

		// CardArray
		CardArray: CardArray,

		// Deck
		Deck: Deck,

		// Hand
		Hand: Hand,

		// HandList
		HandList: HandList
	};

})();

var lca = LL.CardArray;
var a = new lca("s9 d8 s7 h6 s5 c4");
var card = LL.Card.make(LL.Card.RANK_2 | LL.Card.SUIT_HEART)
a.removeCard(card);
a.sort();

var ld = LL.Deck;
var d = new ld();
var s = d.deal(5);
console.log(s.toString());
console.log(d.cards.toString());
d.recycle(s);
d.used.reverse();
var c = d.used.clone();
console.log(c.toString());
d.reset();
d.shuffle();
console.log(d.cards.toString());

var ht = "♦6 ♠6 ♥7 ♠7 ♦8 ♣8 ♣9 ♦9";

var lh = LL.Hand;
var h = new lh();
h.parse(new lca(ht));
console.log(h.toString());

console.log(a.length);