/**
 * Calls the user_registry_example in an attempt to register a username
 * 
 * Be sure to set rpcUrl below to point to the cluster you've deployed the program to.
 */

const web3 = require("@solana/web3.js");

const rpcUrl = web3.clusterApiUrl("devnet");
//const rpcUrl = "http://localhost:8899"; // e.g. when running a solana-test-validator

if (process.argv.length != 4) {
    console.error("ERROR: Wrong arguments. Run this script as follows:");
    console.error("npm run user_registry_example <programId> <username>");
    process.exit(1);
}

const programId = new web3.PublicKey(process.argv[2]);
const username = process.argv[3];

const memoPubkey = new web3.PublicKey("MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr");

async function register_username() {
    let payer = new web3.Keypair();

    const airdropSig = await connection.requestAirdrop(payer.publicKey, web3.LAMPORTS_PER_SOL);
    await connection.confirmTransaction(airdropSig);

    // create the PDA for the account
    const usernameBuffer = Buffer.from(username);
    const [pda, bumpSeed] = await web3.PublicKey.findProgramAddress([usernameBuffer], programId);
    
    let transaction = new web3.Transaction();

    // fund the account
    const PUBKEY_BYTE_COUNT = 32;
    const lamports = await connection.getMinimumBalanceForRentExemption(PUBKEY_BYTE_COUNT);
    transaction.add(web3.SystemProgram.transfer({fromPubkey: payer.publicKey, lamports: lamports, toPubkey: pda}));

    transaction.add(new web3.TransactionInstruction({
        keys: [
            {pubkey: payer.publicKey, isSigner: true, isWritable: false},
            {pubkey: pda, isSigner: false, isWritable: true},
            {pubkey: web3.SystemProgram.programId, isSigner: false, isWritable: false},
            {pubkey: memoPubkey, isSigner: false, isWritable: false},
        ],
        programId,
        data: usernameBuffer,
    }));

    return web3.sendAndConfirmTransaction(connection, transaction, [payer]);
}

let connection = new web3.Connection(rpcUrl, "processed");
register_username().then(console.log);
