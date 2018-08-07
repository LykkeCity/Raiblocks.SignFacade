#include "sign_service.hpp"
#include <ed25519-donna/ed25519.h>
#include <rai/lib/blocks.hpp>
#include <rai/node/node.hpp>

extern "C" char * key_create()
{
	rai::keypair pair;
	std::string private_key = pair.prv.data.to_string ();
	std::string account = pair.pub.to_account ();
	std::string res = private_key+":"+account;
	return strcpy(new char[res.size()], res.c_str());
}

std::string block_create (
	std::string type_text, 
	std::string account_text,
	std::string representative_text,
	std::string link_text,
	std::string balance_text,
	std::string previous_text,
	std::string work_text,
	std::string key_text,
	int* errorCode);

std::string getString(char * str)
{
	if (str == NULL)
	{
		return "";
	}
	
	std::string res(str);
	return res;
}

extern "C" char * block_create_c (
	char * type_text, 
	char * account_text,
	char * representative_text,
	char * link_text,
	char * balance_text,
	char * previous_text,
	char * work_text,
	char * key_text,
	int  * errorCode)
{
	std::string type = getString(type_text);
	std::string account = getString(account_text);
	std::string representative = getString(representative_text);
	std::string link = getString(link_text);
	std::string balance = getString(balance_text);
	std::string previous = getString(previous_text);
	std::string work = getString(work_text);
	std::string key = getString(key_text);

	*errorCode = result_ok;
	std::string res = block_create(type, account, representative, link, balance, previous, work, key, errorCode);
    return strcpy(new char[res.size()], res.c_str());
}

std::string block_create (
	std::string type_text, 
	std::string account_text,
	std::string representative_text,
	std::string link_text,
	std::string balance_text,
	std::string previous_text,
	std::string work_text,
	std::string key_text,
	int* errorCode)
{
	*errorCode = result_err; 
    if (type_text != "state")
	{
		return "Unsupported block type";
	}

	rai::uint256_union account (0);

	if (!account_text.empty())
	{
		auto error_account (account.decode_account (account_text));
		if (error_account)
		{
			return "Bad account number";
		}
	}
	else
	{
		return  "Empty account number";
	}

	rai::uint256_union representative (0);
	if (!representative_text.empty())
	{
		auto error_representative (representative.decode_account (representative_text));
		if (error_representative)
		{
			return "Bad representative account";
		}
	}
	else
	{
		return  "Empty representative account";
	}

	rai::uint256_union link (0);
	if (!link_text.empty())
	{
		auto error_link (link.decode_account (link_text));
		if (error_link)
		{
			auto error_link (link.decode_hex (link_text));
			if (error_link)
			{
				return  "Bad link number";
			}
		}
	}
	else
	{
		return  "Empty link number";
	}

	rai::uint128_union balance (0);
	if (!balance_text.empty ())
	{
		auto error_balance (balance.decode_dec (balance_text));
		if (error_balance)
		{
			return "Bad balance number";
		}
	}
	else
	{
		return  "Empty balance";
	}

	rai::uint256_union previous (0);
	if (!previous_text.empty())
	{
		auto error_previous (previous.decode_hex (previous_text));
		if (error_previous)
		{
			return "Invalid previous hash";
		}
	}
	else
	{
		return  "Empty previous hash";
	}

	uint64_t work (0);
	rai::from_string_hex (work_text, work);
	if (!work_text.empty())
	{
		auto work_error (rai::from_string_hex (work_text, work));
		if (work_error || work == 0)
		{
			return "Bad work";
		}
	}
	else
	{
		return  "Empty work";
	}

	rai::raw_key prv;
	prv.data.clear ();
	if (!key_text.empty())
	{
		auto error_key (prv.data.decode_hex (key_text));
		if (error_key || prv.data == 0)
		{
			return "Bad private key";
		}
	}
	else
	{
		return  "Empty key";
	}

	rai::uint256_union pub;
	ed25519_publickey (prv.data.bytes.data (), pub.bytes.data ());
	if (!account_text.empty())
	{
		if (account != pub)
		{
			return "Incorrect key for given account";
		}
	}
	else
	{
		return  "Empty account";
	}

	rai::state_block state (pub, previous, representative, balance, link, prv, pub, work);
	std::string block;
	state.serialize_json (block);
	*errorCode = result_ok;
	return block;
}