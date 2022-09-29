using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace Server.Migrations
{
    public partial class InitialCreate : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Testpersons",
                columns: table => new
                {
                    Id = table.Column<int>(type: "INTEGER", nullable: false)
                        .Annotation("Sqlite:Autoincrement", true),
                    anonymName = table.Column<string>(type: "TEXT", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Testpersons", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "SUSTests",
                columns: table => new
                {
                    Id = table.Column<int>(type: "INTEGER", nullable: false)
                        .Annotation("Sqlite:Autoincrement", true),
                    Question = table.Column<string>(type: "TEXT", nullable: true),
                    StronglyDisagree = table.Column<bool>(type: "INTEGER", nullable: false),
                    Disagree = table.Column<bool>(type: "INTEGER", nullable: false),
                    Neutral = table.Column<bool>(type: "INTEGER", nullable: false),
                    Agree = table.Column<bool>(type: "INTEGER", nullable: false),
                    StronglyAgree = table.Column<bool>(type: "INTEGER", nullable: false),
                    TestpersonId = table.Column<int>(type: "INTEGER", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_SUSTests", x => x.Id);
                    table.ForeignKey(
                        name: "FK_SUSTests_Testpersons_TestpersonId",
                        column: x => x.TestpersonId,
                        principalTable: "Testpersons",
                        principalColumn: "Id");
                });

            migrationBuilder.CreateTable(
                name: "UEQTests",
                columns: table => new
                {
                    Id = table.Column<int>(type: "INTEGER", nullable: false)
                        .Annotation("Sqlite:Autoincrement", true),
                    LeftWord = table.Column<string>(type: "TEXT", nullable: true),
                    RightWord = table.Column<string>(type: "TEXT", nullable: true),
                    one = table.Column<bool>(type: "INTEGER", nullable: false),
                    two = table.Column<bool>(type: "INTEGER", nullable: false),
                    three = table.Column<bool>(type: "INTEGER", nullable: false),
                    four = table.Column<bool>(type: "INTEGER", nullable: false),
                    five = table.Column<bool>(type: "INTEGER", nullable: false),
                    six = table.Column<bool>(type: "INTEGER", nullable: false),
                    seven = table.Column<bool>(type: "INTEGER", nullable: false),
                    TestpersonId = table.Column<int>(type: "INTEGER", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_UEQTests", x => x.Id);
                    table.ForeignKey(
                        name: "FK_UEQTests_Testpersons_TestpersonId",
                        column: x => x.TestpersonId,
                        principalTable: "Testpersons",
                        principalColumn: "Id");
                });

            migrationBuilder.CreateTable(
                name: "VisAWITests",
                columns: table => new
                {
                    Id = table.Column<int>(type: "INTEGER", nullable: false)
                        .Annotation("Sqlite:Autoincrement", true),
                    Question = table.Column<string>(type: "TEXT", nullable: true),
                    DisagreeEtAll = table.Column<bool>(type: "INTEGER", nullable: false),
                    Disagree = table.Column<bool>(type: "INTEGER", nullable: false),
                    DisagreeRather = table.Column<bool>(type: "INTEGER", nullable: false),
                    Neutral = table.Column<bool>(type: "INTEGER", nullable: false),
                    AgreeRather = table.Column<bool>(type: "INTEGER", nullable: false),
                    Agree = table.Column<bool>(type: "INTEGER", nullable: false),
                    AgreeEtAll = table.Column<bool>(type: "INTEGER", nullable: false),
                    TestpersonId = table.Column<int>(type: "INTEGER", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_VisAWITests", x => x.Id);
                    table.ForeignKey(
                        name: "FK_VisAWITests_Testpersons_TestpersonId",
                        column: x => x.TestpersonId,
                        principalTable: "Testpersons",
                        principalColumn: "Id");
                });

            migrationBuilder.CreateIndex(
                name: "IX_SUSTests_TestpersonId",
                table: "SUSTests",
                column: "TestpersonId");

            migrationBuilder.CreateIndex(
                name: "IX_UEQTests_TestpersonId",
                table: "UEQTests",
                column: "TestpersonId");

            migrationBuilder.CreateIndex(
                name: "IX_VisAWITests_TestpersonId",
                table: "VisAWITests",
                column: "TestpersonId");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "SUSTests");

            migrationBuilder.DropTable(
                name: "UEQTests");

            migrationBuilder.DropTable(
                name: "VisAWITests");

            migrationBuilder.DropTable(
                name: "Testpersons");
        }
    }
}
