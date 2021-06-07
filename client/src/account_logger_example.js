/**
 * A simple client script that calls the account_logger_example to log the Clock sysvar
 * 
 * Be sure to set rpcUrl below to point to the cluster you've deployed the program to.
 * 
 * The logged output can be found in multiple places:
 * * This script subscribes to the logs and prints them.
 * * It is printed by `solana logs -ul` when run together with a solana-test-validator.
 * * You may search for the transaction signature on explorer.solana.com.
 */

const web3 = require("@solana/web3.js");

const rpcUrl = web3.clusterApiUrl("devnet");
//const rpcUrl = "http://localhost:8899"; // e.g. when running a solana-test-validator

if (process.argv.length != 3) {
    console.error("ERROR: Missing the programId argument. Run this script as follows:");
    console.error("npm run account_logger_example <programId>");
    process.exit(1);
}

const programId = new web3.PublicKey(process.argv[2]);

async function log_clock_account() {
    let payer = new web3.Keypair();

    const airdropSig = await connection.requestAirdrop(payer.publicKey, web3.LAMPORTS_PER_SOL);
    await connection.confirmTransaction(airdropSig);

    let transaction = new web3.Transaction().add(new web3.TransactionInstruction({
        keys: [
            {pubkey: web3.SYSVAR_CLOCK_PUBKEY, isSigner: false, isWritable: false},
        ],
        programId
    }));

    return web3.sendAndConfirmTransaction(connection, transaction, [payer]);
}

let connection = new web3.Connection(rpcUrl, "processed");
const subscriptionId = connection.onLogs(programId, console.log);
log_clock_account().then(setTimeout(() => {connection.removeOnLogsListener(subscriptionId);}, 5000));
