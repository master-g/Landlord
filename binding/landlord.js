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

	// CardArray

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
			var rcards = [];
			if (rhs.constructor === CardArray) {
				rcards = rhs.cards;
			}
			else if (rhs.constructor === Array) {
				rcards = rhs;
			}

			this.cards = this.cards.concat(rcards);
		},

		subtract: function(rhs) {
			var rcards = [];
			if (rhs.constructor === CardArray) {
				rcards = rhs.cards.slice(0);
			}
			else if (rhs.constructor === Array) {
				rcards = rhs.slice(0);
			}

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

			var rcards = [];
			if (rhs.constructor === Array) {
				rcards = rhs.slice(0);
			}
			else if (rhs.constructor === this.constructor) {
				rcards = rhs.cards.slice(0);
			}

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

			var rcards = [];
			if (rhs.constructor === Array) {
				rcards = rhs.slice(0);
			}
			else if (rhs.constructor === this.constructor) {
				rcards = rhs.cards.slice(0);
			}

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
		}
	};

	CardArray.prototype.constructor = CardArray;

	// Deck
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

	// Hand

	var Hand = function() {
		this.type = 0;
		this.cards = new CardArray();
	};

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


	// distribute cards
	// for example, Hand.Distribute(xxx, 88666644, 422, 4, 2, 8)
	// result in 66668844
	Hand.Distribute = function(hand, arr, count, d1, d2, length) {
		var temp = new CardArray();
		var cards = arr.cards;

		for (var i = 0; i < cards.length; i++) {
			var card = cards[i];
			var num = count[Card.getRank(card)];

			if (num === d1) {
				hand.cards.push_back(card);
			}
			else if (num === d2) {
				temp.push_back(card);
			}

			if (hand.cards.size() + temp.size() >= length) {
				hand.cards.concat(temp);
				break;
			}
		}
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

		}

	};

	Hand.prototype.constructor = Hand;

	return {

		Card: Card,

		// CardArray
		CardArray: CardArray,

		// Deck
		Deck: Deck,

		// Hand
		Hand: Hand,
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

console.log(LL.Hand);

var s = [3,5,2,1,6,4];
s.sort();
s.reverse();
console.log(s);